#include "cc_grabber.h"

// ================================================================
// Constructor
// ================================================================
CCGrabber::CCGrabber(string video_device, string video_format)
    : Initialised(false)
{
    // Set device
    Video_device = video_device;

    // Set format
    Video_format = video_format;

    // Set image pointer to NULL
    Image_pt = NULL;

}

// ================================================================
// Destructor
// ================================================================
CCGrabber::~CCGrabber()
{
    if (Initialised)
    {
        delete [] Image_pt;
        Image_pt = NULL;
    }
}

// ================================================================
// ================================================================
int CCGrabber::initialise_video()
{
    if (!Video_format.compare("NTSC")) // returns 0 if strings are equal
    {
        Width = 720;// 640;
        Height = 480;//480;
        N_channels = 3;
        Video_device_standard_for_v4l2 = V4L2_STD_NTSC;
    }
    else if (!Video_format.compare("PAL")) // returns 0 if strings are equal
    {
        Width = 720;
        Height = 576;
        N_channels = 3;
        Video_device_standard_for_v4l2 = V4L2_STD_PAL;
    }
    else // Non supported video format
    {
        Initialised = false;
        return false;
    }

    // -----------------------------------
    // Initiliase video stage
    // -----------------------------------
    // Open the device
    open_device (&FD, (char*)Video_device.c_str());

    // Set v4l2 standard
    set_standard(&FD, Video_device_standard_for_v4l2);

    // Initialise device
    buffers = init_device(&FD, (char*)Video_device.c_str(), Width, Height, &n_buffers, pixel_format);

    // Create storage for temporary image storage
    temp_img = new unsigned int[Width*Height];

    start_capturing (&FD, &n_buffers);

    // Allocate memory for image storage
    Image_pt = new unsigned char[Width*Height*N_channels];

    Initialised = true;
    return true;

}

// ================================================================
// Reads a frame from the frame grabber and stores into an
// unsigned char * variable
// ================================================================
int CCGrabber::read_frame()
{

    // Security check
    if (!Initialised)
     {
        return false;
     }

    int r;

    // Initialise stuff
    FD_ZERO (&FDS);
    FD_SET (FD, &FDS);
    TV.tv_sec = 2;
    TV.tv_usec = 0;

    // FD is in grabber.cpp !!!!!!!
    r = select (FD + 1, &FDS, NULL, NULL, &TV);
    if (r == -1)
     {
      GRABBER_ERROR("select");
     }

    if (r == 0)
     {
      fprintf (stderr, "select timeout\n");
      //continue;
     }

        struct v4l2_buffer buf;//needed for memory mapping
        //unsigned int i;
        //unsigned int Bpf;//bytes per frame

        CLEAR (buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;

        if (-1 == xioctl ((FD), VIDIOC_DQBUF, &buf))
        {
                switch (errno)
                {
                        case EAGAIN:
                                return 0;

                        case EIO://EIO ignored

                        default:
                                MABESA_ERROR ("VIDIOC_DQBUF");
                }
        }

        assert (buf.index < n_buffers);

        //Bpf = width*height*2;

        memcpy(temp_img,buffers[buf.index].start,Width*Height*2);

        process2a(temp_img, Width, Height);
        //medicion (textura, 70, &prom_tela, &desv_tela, &calidad_tela);


        //pthread_mutex_lock(&myMutex);
        //pthread_cond_broadcast(&mySignal);
        //pthread_mutex_unlock(&myMutex);

        if (-1 == xioctl (FD, VIDIOC_QBUF, &buf))
                MABESA_ERROR ("VIDIOC_QBUF");

        return 1;

}

// ================================================================
// ================================================================
int CCGrabber::xioctl (int fd, int request, void *arg)
{
        int r;

        do r = ioctl (fd, request, arg);
        while (-1 == r && EINTR == errno);

        return r;
}

// ================================================================
// ================================================================
sbuffer *CCGrabber::init_mmap (int * fd, char * dev_name, int * n_buffers)
{
        struct v4l2_requestbuffers req;
        //buffers is an array of n_buffers length, and every element store a frame
        sbuffer *buffers = NULL;
        CLEAR (req);

        req.count               = 4;
        req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory              = V4L2_MEMORY_MMAP;

        if (-1 == xioctl (*fd, VIDIOC_REQBUFS, &req))
        {
                if (EINVAL == errno)
                {
                        fprintf (stderr, "%s does not support "
                                                                "memory mapping\n", dev_name);
                        exit (EXIT_FAILURE);
                } else {
                        MABESA_ERROR ("VIDIOC_REQBUFS");
                }
        }

        if (req.count < 2)
        {
                fprintf (stderr, "Insufficient buffer memory on %s\n",dev_name);
                exit (EXIT_FAILURE);
        }
        buffers = (sbuffer*)calloc (req.count, sizeof (*buffers));
        if (!buffers)
        {
                fprintf (stderr, "Out of memory\n");
                exit (EXIT_FAILURE);
        }
        //map every element of the array buffers to the shared memory
        for (*n_buffers = 0; *n_buffers < req.count; ++*n_buffers)
        {
                struct v4l2_buffer buf;

                CLEAR (buf);

                buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory      = V4L2_MEMORY_MMAP;
                buf.index       = *n_buffers;

                if (-1 == xioctl (*fd, VIDIOC_QUERYBUF, &buf))
                        MABESA_ERROR ("VIDIOC_QUERYBUF");

                buffers[*n_buffers].length = buf.length;
                buffers[*n_buffers].start = mmap (NULL /* start anywhere */,
                                                        buf.length,
                                                        PROT_READ | PROT_WRITE /* required */,
                                                        MAP_SHARED /* recommended */,
                                                        *fd, buf.m.offset);

                if (MAP_FAILED == buffers[*n_buffers].start)
                        MABESA_ERROR ("mmap");
        }
        return buffers;
}

// ================================================================
// ================================================================
void CCGrabber::open_device (int * fd, char * dev_name)
{

        struct stat st;

        if (-1 == stat (dev_name, &st))
        {
                fprintf (stderr, "Cannot identify '%s': %d, %s\n",dev_name, errno, strerror (errno));
                        exit (EXIT_FAILURE);
        }

        if (!S_ISCHR (st.st_mode))
        {
                fprintf (stderr, "%s is no device\n", dev_name);
                        exit (EXIT_FAILURE);
        }

        *fd = open (dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);

        if (-1 == *fd)
        {
                fprintf (stderr, "Cannot open '%s': %d, %s\n",dev_name, errno, strerror (errno));
                exit (EXIT_FAILURE);
        }
}

// ================================================================
// ================================================================
void CCGrabber::set_standard(int * fd, int dev_standard)
{
        struct v4l2_standard standard;
        v4l2_std_id st;
        standard.index = dev_standard;;
        /*if (-1 == ioctl (*fd, VIDIOC_ENUMSTD, &standard))
        {
                perror ("VIDIOC_ENUMSTD");
        }*/
        st=dev_standard;//standard.id;

        if (-1 == ioctl (*fd, VIDIOC_S_STD, &st))
        {
                perror ("VIDIOC_S_STD");
        }
        fprintf (stderr,"standard: %s\n", standard.name);
}

// ================================================================
// ================================================================
sbuffer *CCGrabber::init_device (int * fd, char * dev_name, int width, int height, int * n_buffers, int pixel_format)
{
        struct v4l2_capability cap;
        struct v4l2_cropcap cropcap;
        struct v4l2_crop crop;
        struct v4l2_format fmt;
        sbuffer * buffers = NULL;
        unsigned int min;

        if (-1 == xioctl (*fd, VIDIOC_QUERYCAP, &cap))
        {
                if (EINVAL == errno)
                {
                        fprintf (stderr, "%s is no V4L2 device\n", dev_name);
                        exit (EXIT_FAILURE);
                } else {
                        MABESA_ERROR ("VIDIOC_QUERYCAP");
                }
        }

        if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
        {
                fprintf (stderr, "%s is no video capture device\n",dev_name);
                exit (EXIT_FAILURE);
        }

        if (!(cap.capabilities & V4L2_CAP_STREAMING))
        {
                fprintf (stderr, "%s does not support streaming i/o\n",dev_name);
                exit (EXIT_FAILURE);
        }

        /* Select video input, video standard and tune here. */
        cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (-1 == xioctl (*fd, VIDIOC_CROPCAP, &cropcap))
        {
                                /* Errors ignored. */
                                fprintf (stderr,"test\n");
        }

        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect; /* reset to default */

        if (-1 == xioctl (*fd, VIDIOC_S_CROP, &crop))
        {
                switch (errno) {
                        case EINVAL:
                                /* Cropping not supported. */
                        break;
                        default:
                                /* Errors ignored. */
                        break;
                }

        }

        CLEAR (fmt);
        //set image properties
        fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        // 720 x 480
        fmt.fmt.pix.width       = width;
        fmt.fmt.pix.height      = height;


        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV ;

        //fmt.fmt.pix.colorspace  = V4L2_COLORSPACE_SRGB;
        //fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

        if (-1 == xioctl (*fd, VIDIOC_S_FMT, &fmt))
                MABESA_ERROR ("\nError: pixel format not supported\n");

        /* Note VIDIOC_S_FMT may change width and height. */

        //check the configuration data
        min = fmt.fmt.pix.width * 2;
        if (fmt.fmt.pix.bytesperline < min)
                        fmt.fmt.pix.bytesperline = min;
        min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
        if (fmt.fmt.pix.sizeimage < min)
                        fmt.fmt.pix.sizeimage = min;
        #if (DEBUG_GARFIO)
        fprintf(stderr, "Video bytespreline = %d\n",fmt.fmt.pix.bytesperline);
        #endif
        buffers = init_mmap (fd, dev_name, n_buffers);

        return buffers;
}

// ================================================================
// ================================================================
void CCGrabber::start_capturing (int * fd, int * n_buffers )
{
        unsigned int i;
        enum v4l2_buf_type type;

        for (i = 0; i < *n_buffers; ++i)
        {
                struct v4l2_buffer buf;

                CLEAR(buf);

                buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory      = V4L2_MEMORY_MMAP;
                buf.index       = i;

                if (-1 == xioctl (*fd, VIDIOC_QBUF, &buf))
                        MABESA_ERROR ("VIDIOC_QBUF");
        }

        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        //start the capture from the device
        if (-1 == xioctl (*fd, VIDIOC_STREAMON, &type))
                MABESA_ERROR ("VIDIOC_STREAMON");
}

// ================================================================
// ================================================================
void CCGrabber::stop_capturing (int * fd)
{
        enum v4l2_buf_type type;

        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        //this call to xioctl allows to stop the stream from the capture device
        if (-1 == xioctl (*fd, VIDIOC_STREAMOFF, &type))
                MABESA_ERROR ("VIDIOC_STREAMOFF");
}

// ================================================================
// ================================================================
int CCGrabber::yuv2rgb(int y, int u, int v, char *r, char *g, char *b)
{

   int r1, g1, b1;
   int c = y-16, d = u - 128, e = v - 128;

   r1 = (298 * c           + 409 * e + 128) >> 8;
   g1 = (298 * c - 100 * d - 208 * e + 128) >> 8;

   b1 = (298 * c + 516 * d           + 128) >> 8;

   // Even with proper conversion, some values still need clipping.

   if (r1 > 255) r1 = 255;
   if (g1 > 255) g1 = 255;
   if (b1 > 255) b1 = 255;
   if (r1 < 0) r1 = 0;
   if (g1 < 0) g1 = 0;
   if (b1 < 0) b1 = 0;

   *r = r1 ;
   *g = g1 ;
   *b = b1 ;
}

// ================================================================
// ================================================================
void CCGrabber::process2(unsigned int *start, int w, int h)
{

//here is a possible way to remove the interlace from the grabber
        int i,j;
        unsigned int *pixel_16;     // for YUYV
        unsigned char *pixel_24;    // for RGB
        unsigned char *pixel_24_AUX;
        int y, u, v, y2;
        char r, g, b;

        pixel_16 = (unsigned int *)start;
        pixel_24 = &Image_pt[0];

        //int res=(int)pixel_24;

        int conttext=0;

        fprintf(stderr, "fg\n");
        for (i=0;i<h;i+=2)
        {

                pixel_24_AUX=pixel_24+w*3;
                for (j=0;j<w;j+=2)
                {
                        v  = ((*pixel_16 & 0x000000ff));
                        y  = ((*pixel_16 & 0x0000ff00)>>8);
                        u  = ((*pixel_16 & 0x00ff0000)>>16);
                        y2 = ((*pixel_16 & 0xff000000)>>24);
                        y2  = ((*pixel_16 & 0x000000ff));
                        u  = ((*pixel_16 & 0x0000ff00)>>8);
                        y  = ((*pixel_16 & 0x00ff0000)>>16);
                        v = ((*pixel_16 & 0xff000000)>>24);
                        yuv2rgb(y, u, v, &r, &g, &b);


                   /*if ((i==h/2)&&(j<(w/2+80))&&(j>(w/2-80))&&(idx==0))
                   {
                   //r=g=b=0;
                       //int promrgb = (r+g+b)/3;
                       //

                       //textura[conttext] = (conttext%4) * 1;
                       textura[conttext] = (unsigned char)r;

                       //textura[conttext+1] = promrgb;
                       conttext++;
                       //r=g=b=0;
                   }*/

                    *pixel_24++ = r;
                    *pixel_24++ = g;
                    *pixel_24++ = b;

                    *pixel_24_AUX++ = r;
                    *pixel_24_AUX++ = g;
                    *pixel_24_AUX++ = b;


                    /*yuv2rgb(y2, u, v, &r, &g, &b);            // 2nd pixel

                    if ((i==h/2)&&(j<(w/2+80))&&(j>(w/2-80)))
                    {
                        r=g=b=0;
                    }*/

                    *pixel_24++ = r;
                    *pixel_24++ = g;
                    *pixel_24++ = b;

                    *pixel_24_AUX++ = r;
                    *pixel_24_AUX++ = g;
                    *pixel_24_AUX++ = b;

                    pixel_16++;

                }
                pixel_24=pixel_24_AUX;
                pixel_16+=w/2;
        }
        //fprintf(stderr, "%d %d - %d\n", idx, pixel_24, res-(int)pixel_24);


#if 0
        int i;
        unsigned int *pixel_16;     // for YUYV
        unsigned char *pixel_24;    // for RGB
        int y, u, v, y2;
        char r, g, b;
//   unsigned char *ptr = 0;

    pixel_16 = (unsigned int *)start;
    pixel_24 = frames[idx]->image;   // + bih.biWidth*bih.biHeight * 3

    for (i=0; i< w*h/2 ; i++) {

        v  = ((*pixel_16 & 0x000000ff));
        y  = ((*pixel_16 & 0x0000ff00)>>8);
        u  = ((*pixel_16 & 0x00ff0000)>>16);
        y2 = ((*pixel_16 & 0xff000000)>>24);
                  y2  = ((*pixel_16 & 0x000000ff));
        u  = ((*pixel_16 & 0x0000ff00)>>8);
        y  = ((*pixel_16 & 0x00ff0000)>>16);
        v = ((*pixel_16 & 0xff000000)>>24);

        yuv2rgb(y, u, v, &r, &g, &b);            // 1st pixel

        *pixel_24++ = r;
        *pixel_24++ = g;
        *pixel_24++ = b;

        yuv2rgb(y2, u, v, &r, &g, &b);            // 2nd pixel

        *pixel_24++ = r;
        *pixel_24++ = g;
        *pixel_24++ = b;

        pixel_16++;
    }
#endif
}

// ================================================================
// ================================================================
void CCGrabber::process2a(unsigned int *start, int w, int h)
{

//here is a possible way to remove the interlace from the grabber
        int i,j,idx=0, offy=2;
        unsigned int *pixel_16;     // for YUYV
        unsigned char *pixel_24;    // for RGB
        unsigned char *pixel_24_l;    // for RGB
        unsigned char *pixel_24_resp;
        unsigned char *pixel_24_AUX;
        int y, u, v, y2;
        char r, g, b;

        pixel_16 = (unsigned int *)start;
        pixel_24 = Image_pt;//frames->image;
        pixel_24_l = data_video_l;//frames->image;

        //int res=(int)pixel_24;

        int conttext=0;

        for (i=0;i<h;i+=2)
        {

                pixel_24_AUX=pixel_24+w*3;
                for (j=0;j<w;j+=2)
                {
                        v  = ((*pixel_16 & 0x000000ff));
                        y  = ((*pixel_16 & 0x0000ff00)>>8);
                        u  = ((*pixel_16 & 0x00ff0000)>>16);
                        y2 = ((*pixel_16 & 0xff000000)>>24);
                        y2  = ((*pixel_16 & 0x000000ff));
                        u  = ((*pixel_16 & 0x0000ff00)>>8);
                        y  = ((*pixel_16 & 0x00ff0000)>>16);
                        v = ((*pixel_16 & 0xff000000)>>24);
                        yuv2rgb(y, u, v, &r, &g, &b);


                   if ((i==(h/2 + offy ))&&(j<(w/2+80))&&(j>(w/2-80))&&(idx==0))
                   {
                       conttext++;
                       textura[conttext] = ((unsigned char)r+(unsigned char)g+(unsigned char)b) /3 ;

                       if ((conttext==1)||(conttext==79))
                       {
                           r=0;
                           g=0;
                           b=255;
                       }
                   }

                   if ((i==(h/2-2+offy))&&(j<(w/2+80))&&(j>(w/2-80))&&(idx==0))
                   {
                       r=0;
                       g=0;
                       b=255;
                   }

                   if ((i==(h/2+2+offy))&&(j<(w/2+80))&&(j>(w/2-80))&&(idx==0))
                   {
                       r=0;
                       g=255;
                       b=255;
                   }

                   *pixel_24_l++ = r;
                   *pixel_24_l++ = g;
                   *pixel_24_l++ = b;

                    *pixel_24++ = r;
                    *pixel_24++ = g;
                    *pixel_24++ = b;

                    *pixel_24_AUX++ = r;
                    *pixel_24_AUX++ = g;
                    *pixel_24_AUX++ = b;

                    *pixel_24++ = r;
                    *pixel_24++ = g;
                    *pixel_24++ = b;

                    *pixel_24_AUX++ = r;
                    *pixel_24_AUX++ = g;
                    *pixel_24_AUX++ = b;

                    pixel_16++;

                }
                pixel_24=pixel_24_AUX;
                pixel_16+=w/2;
        }

}
