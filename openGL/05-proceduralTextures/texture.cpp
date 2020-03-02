/* texture.C
 *
 * Needs the Devil image library.
 *
 * On Linux: sudo apt-get install libdevil-dev
 *
 * On Windows, get code from https://github.com/DentonW/DevIL
 */


#include "headers.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <math.h>

#ifdef _UNICODE
  #include "atlstr.h"
#endif

#include "texture.h"

using namespace std;

bool Texture::useMipMaps = false;


/* Register the current texture with OpenGL, assigning
 * it a textureID.
 */

void Texture::registerWithOpenGL()

{
  // Register it with OpenGL

  glGenTextures( 1, &textureID );
  glBindTexture( GL_TEXTURE_2D, textureID );

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

  glPixelTransferf( GL_RED_SCALE,   scale );
  glPixelTransferf( GL_GREEN_SCALE, scale );
  glPixelTransferf( GL_BLUE_SCALE,  scale );
    
  glPixelTransferf( GL_RED_BIAS,   bias );
  glPixelTransferf( GL_GREEN_BIAS, bias );
  glPixelTransferf( GL_BLUE_BIAS,  bias );
    
  if (useMipMaps) {  // mip-mapped texture

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    gluBuild2DMipmaps( GL_TEXTURE_2D, 
		       (isGreyscale ? (greyscaleBytesPerPixel==1 ? GL_LUMINANCE8 : GL_LUMINANCE16) : (hasAlpha ? GL_RGBA : GL_RGB)),
		       width, height,
		       (isGreyscale ? GL_LUMINANCE : (hasAlpha ? GL_RGBA : GL_RGB)),
		       (isGreyscale ? (greyscaleBytesPerPixel==1 ? GL_UNSIGNED_BYTE : GL_SHORT) : GL_UNSIGNED_BYTE),
		       texmap );

  } else {  // ordinary texture
 
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexImage2D( GL_TEXTURE_2D,
		  0,
		  (isGreyscale ? (greyscaleBytesPerPixel==1 ? GL_LUMINANCE8 : GL_LUMINANCE16) : (hasAlpha ? GL_RGBA : GL_RGB)),
		  width, height, 0,
		  (isGreyscale ? GL_LUMINANCE : (hasAlpha ? GL_RGBA : GL_RGB)),
		  (isGreyscale ? (greyscaleBytesPerPixel==1 ? GL_UNSIGNED_BYTE : GL_SHORT) : GL_UNSIGNED_BYTE),
		  texmap );
  }
}


// Read a texture from a RAW file of 16-bit greyscale


GLubyte *Texture::readRAW( const char *filename, int xdim, int ydim )

{
  int i;
  unsigned char *a, *b, *pa, *pb;

  ifstream f( filename, ios::binary );

  if (!f) {
    cerr << "Open of `" << filename << "' failed.\n";
    exit(1);
  }

  width = xdim;
  height = ydim;

  // read the data (stored top-to-bottom, left-to-right)

  a = new unsigned char[ xdim * ydim * 2 ];
  f.read( (char *) a, xdim*ydim*2 );

  float min = 99999;
  float max = -99999;
  short int *si = (short int *) a;
  for (int i=0; i< xdim*ydim; i++) {
    if (*si < min) min = *si;
    if (*si > max) max = *si;
    si++;
  }

  // flip the image vertically (stored bottom-to-top, left-to-right)

  b = new unsigned char[ xdim * ydim * 2 ];

  for (i=0; i<ydim; i++) {
    pa = a + (i)*xdim*2;
    pb = b + (ydim-1-i)*xdim*2;
    for (int j=0; j<xdim*2; j++)
      *(pb++) = *(pa++);
  }

  delete [] a;

  hasAlpha = false;
  isGreyscale = true;
  greyscaleBytesPerPixel = 2;

  return b;
}


// Read a texture from an image file

unsigned char *Texture::readImage( const char *filename )

{
  // Create image name

  ILuint ImageName;
  ilGenImages(1, &ImageName);
  ilBindImage(ImageName);

  // Load image

  if (ilGetError() != IL_NO_ERROR)
    cerr << "DevIL error prior to opening '" << filename << "'." << endl;

#if _UNICODE
  ilLoadImage( (const wchar_t *) filename );
#else
  ilLoadImage( filename );
#endif

  ILenum status;
  while ((status = ilGetError()) != IL_NO_ERROR)
    switch (status) {
    case IL_INVALID_EXTENSION:
      cerr << "ERROR: invalid extension when opening '" << filename << "'." << endl;
      break;
    case IL_FORMAT_NOT_SUPPORTED:
      cerr << "ERROR: File format of '" << filename << "' is not supported." << endl;
      break;
    case IL_COULD_NOT_OPEN_FILE:
      cerr << "ERROR: Could not open '" << filename << "'." << endl;
      break;
    default:
      cerr << "ERROR: A problem (code " << status << ") occurred when reading '" << filename << "'." << endl;
      break;
    }

  // Copy into local buffer

  width = ilGetInteger(IL_IMAGE_WIDTH);
  height = ilGetInteger(IL_IMAGE_HEIGHT);

  ILubyte *buff = new ILubyte[ width * height * 4 ];

  ilCopyPixels( 0, 0, 0,	  // x,y,z offsets
		width, height, 1, // x,y,z dimensions
		IL_RGBA,          // type of data returned
		IL_UNSIGNED_BYTE, // type of buffer
		buff );

  hasAlpha = true;
  isGreyscale = false;

  return buff;
}


// Find the texel at [i][j] for i,j in [0,1]x[0,1] of the *image*, *not* of the 2^k x 2^l texture.
//
// Use bilinear interpolation.


vector Texture::texelInterp( float i, float j )

{
  float opacity;

  int x = (int) floor( i*width - 0.5 );
  int y = (int) floor( j*height - 0.5 );

  float alpha = (i*width - 0.5) - x;
  float beta  = (j*height - 0.5) - y;

  return ((1-alpha)*(1-beta)) * texel( x  , y  , opacity )
       + ((1-alpha)*   beta ) * texel( x  , y+1, opacity )
       + (   alpha *(1-beta)) * texel( x+1, y  , opacity )
       + (   alpha *   beta ) * texel( x+1, y+1, opacity );
}


// Find the texel at [x,y] for x,y in [0,xdim-1]x[0,ydim-1]

vector Texture::texel( int x, int y )

{
  float alpha;
  return texel(x,y,alpha);
}


vector Texture::texel( int x, int y, float &alpha )

{
  int bytesPerPixel = (isGreyscale ? greyscaleBytesPerPixel : 3) + (hasAlpha ? 1 : 0);

  if (x<0) x = 0;
  if (x>width-1) x = width-1;
  if (y<0) y = 0;
  if (y>height-1) y = height-1;

  unsigned char *p;
  vector colour;

  //y = height-1-y;

  p = texmap + bytesPerPixel * (y*width + x);

  if (isGreyscale) {

    if (greyscaleBytesPerPixel == 1)
      colour.x = *p;
    else
      colour.x = *((short int *) p);

    colour.y = colour.z = colour.x;

  } else {
    
    colour.x = (*p++)/255.0f;
    colour.y = (*p++)/255.0f;
    colour.z = (*p++)/255.0f;

    if (hasAlpha)
      alpha = (*p++)/255.0f;
    else
      alpha = 1;
  }

  return colour;
}


float Texture::texelLum( int x, int y )

{
  float alpha;
  
  vector colour = texel(x,y,alpha);

  return 0.299 * colour.x + 0.587 * colour.y + 0.114 * colour.z;
}



// Find NCC of neighbourhood around the pixel coords


float matchPixels( Texture &t1, vector &p1, Texture &t2, vector &p2, int KERNEL_HALFWIDTH )

{
  int x1 = (int) p1.x;
  int y1 = (int) p1.y;
  int x2 = (int) p2.x;
  int y2 = (int) p2.y;

  // Find the means

  float t1mean = 0;
  float t2mean = 0;

  for (int i=-KERNEL_HALFWIDTH; i<=KERNEL_HALFWIDTH; i++)
    for (int j=-KERNEL_HALFWIDTH; j<=KERNEL_HALFWIDTH; j++) {
      t1mean += t1.texelLum(x1+i,y1+j);
      t2mean += t2.texelLum(x2+i,y2+j);
    }

  t1mean /= (2*KERNEL_HALFWIDTH+1)*(2*KERNEL_HALFWIDTH+1);
  t2mean /= (2*KERNEL_HALFWIDTH+1)*(2*KERNEL_HALFWIDTH+1);

  // Find the numerator and denominators

  float num = 0;
  float den1 = 0;
  float den2 = 0;

  for (int i=-KERNEL_HALFWIDTH; i<=KERNEL_HALFWIDTH; i++)
    for (int j=-KERNEL_HALFWIDTH; j<=KERNEL_HALFWIDTH; j++) {

      float norm1 = t1.texelLum(x1+i,y1+j) - t1mean;
      float norm2 = t2.texelLum(x2+i,y2+j) - t2mean;

      num += norm1 * norm2;
      den1 += norm1 * norm1;
      den2 += norm2 * norm2;
    }

  return num / sqrt( den1*den2);
}
