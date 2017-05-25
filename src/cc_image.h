#ifndef CC_IMAGE_H
#define CC_IMAGE_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>

// This class represents an image with known width and height,
// the image itself is stored in an "unsigned char pointer" variable
class CCImage
{
 public:
 
 // Constructor, creates an empty image
 CCImage();
 
 // Constructor, allocates space for an image with the specified
 // width, height and pitch
 CCImage(unsigned width, unsigned height, unsigned pitch);
 
 // Destructor
 ~CCImage();
 
 // Copy an image from another CCImage object
 void set_image_as_copy(CCImage* cc_src_image);
 
 // Get a pointer to the internal image storage
 inline unsigned char *image_pt() {return Image_pt;}
 
 // Read-only
 inline const unsigned width() const {return Width;}
 // Write
 inline unsigned &width() {return Width;}
 // Read-only
 inline const unsigned height() const {return Height;}
 // Write
 inline unsigned &height() {return Height;}
 // Read-only
 inline const unsigned pitch() const {return Pitch;}
 // Write
 inline unsigned &pitch() {return Pitch;}
 
 protected:
 
 // Frees image allocated memory
 void free_image_allocated_memory();
 
 // Stores the image
 unsigned char* Image_pt;
 
 // The width of the image
 unsigned Width;
 // The height of the image
 unsigned Height;
 // The pitch of the image (mostly computed as width)
 unsigned Pitch;
 
 // Indicated whether memory was allocated to store the image or not
 bool Allocated_memory;
 
};

#endif // #ifndef CC_IMAGE_H
