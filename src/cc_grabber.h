#ifndef CC_GRABBER_H
#define CC_GRABBER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>             /* getopt_long() */
#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <asm/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>
#include <iostream>

#define CLEAR(x) memset (&(x), 0, sizeof (x))
#define GRABBER_ERROR(x) cerr << ("Error at function "+string(__PRETTY_FUNCTION__)+": "+string(x))
#define MABESA_ERROR(x) cerr << ("Error at function "+string(__PRETTY_FUNCTION__)+": "+string(x))

using namespace std;

struct sbuffer {
        void *start;
        size_t length;
};

#include <string>
#include <math.h>

// This class is in charge of opening, reading and stored a captured image
// from a frame grabber
class CCGrabber
{

public:

    // Constructor
    CCGrabber(string video_device, string video_format);

    // Destructor
    ~CCGrabber();

    // Initialise the video capture
    int initialise_video();

    // Reads a frame from the frame grabber and stores into an
    // unsigned char * variable
    int read_frame();

    // Read access
    inline const int width() const {return Width;}

    // Read access
    inline const int height() const {return Height;}

    // Read access
    inline const int n_channels() const {return N_channels;}

    // Read access
    inline const unsigned char *image_pt() const {return Image_pt;}

    // Returns the number of bytes per line
    inline const unsigned bpl() const {return Width*N_channels;}

    // Read-write access for FD
    inline int &fd() {return FD;}

protected:

    // Indicated whether the grabber was correctly initialised
    bool Initialised;

    // Capture format
    string Video_format;

    // Capture device name
    string Video_device;

    // Image geometry
    int Width;
    int Height;
    int N_channels;

    // Output image is stored here
    unsigned char *Image_pt;

    // File descriptor for low-level methods
    int FD;

    // File descriptors set
    fd_set FDS;

    // A variable for time?
    struct timeval TV;

    // Video device standard for v4l2
    int Video_device_standard_for_v4l2;

    int xioctl (int fd, int request, void *arg);
    sbuffer *init_mmap (int * fd, char * dev_name, int * n_buffers);
    void open_device (int * fd, char * dev_name);
    void set_standard(int * fd, int dev_standard);
    sbuffer *init_device (int * fd, char * dev_name, int width, int height, int * n_buffers, int pixel_format);
    void start_capturing (int * fd, int * n_buffers);
    void stop_capturing (int * fd);

    int yuv2rgb(int y, int u, int v, char *r, char *g, char *b);
    void process2(unsigned int *start, int w, int h);
    void process2a(unsigned int *start, int w, int h);

    int bpp;

    float prom_tela=0, desv_tela=0, calidad_tela=0;

    int dev_input;

    int n_buffers;
    unsigned int textura[320];

    unsigned char data_video_l[360*240*3];

    sbuffer *buffers;
    unsigned int *temp_img;
    //int index;
    int c;
    int pixel_format;

};

#endif // #ifndef CC_GRABBER_H
