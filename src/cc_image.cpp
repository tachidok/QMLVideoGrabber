#include "cc_image.h"

// ============================================================================
// Constructor, creates an empty image
// ============================================================================
CCImage::CCImage()
 : Allocated_memory(false)
{
 // No memory space allocated for this image
 Width = 0;
 Height = 0;
 Pitch = 0;
 
 // Set the image pointer to NULL
 Image_pt = NULL;
}

// ============================================================================
// Constructor, allocates space for an image with the specified width,
// height and pitch
// ============================================================================
CCImage::CCImage(unsigned width, unsigned height, unsigned pitch)
 :
 Allocated_memory(false)
{
 // Set image geometry
 Width = width;
 Height = height;
 Pitch = pitch;
 
 // Set the image pointer to NULL
 Image_pt = NULL;
 
 // Allocate space for the image
 Image_pt = new unsigned char[Width*Height*Pitch]; // TODO: Check
                                                   // whether it
                                                   // should be
                                                   // multiplied by
                                                   // Pitch or the
                                                   // number of
                                                   // channels
 if (Image_pt != NULL)
  {
   Allocated_memory = true;
  }
 
}

// ============================================================================
// Destructor
// ============================================================================
CCImage::~CCImage()
{
 free_image_allocated_memory(); 
}

// Copy an image from another CCImage object
void CCImage::set_image_as_copy(CCImage* cc_src_image)
{ 
 free_image_allocated_memory();
 
 // Number of channels per image
 const unsigned n_channels = 3;
 Width=cc_src_image->width();
 Height=cc_src_image->height();
 Pitch=cc_src_image->width()*n_channels;
 
 // Allocate space for the image
 Image_pt = new unsigned char[Width*Height*n_channels];
 if (Image_pt != NULL)
  {
   Allocated_memory = true;
   memcpy(Image_pt, cc_src_image->image_pt(), Width*Height*n_channels);
  }
 
}

// ============================================================================
// Frees image allocated memory
// ============================================================================
void CCImage::free_image_allocated_memory()
{
 if (Allocated_memory)
  {
   delete [] Image_pt;
   Image_pt = NULL;
   Allocated_memory = false;
  }
}


