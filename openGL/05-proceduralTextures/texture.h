/* texture.h
 */


#ifndef TEXTURE_H
#define TEXTURE_H

#include "headers.h"

#include <string.h>
#include "seq.h"
#include "vector.h"


class Texture {

  GLubyte *texmap;		/* texture map */
  GLuint textureID;		/* the OpenGL ID for this texture */
  bool hasAlpha;		/* true if alpha channel exists */
  bool isGreyscale;             /* true if greyscale */

  int  greyscaleBytesPerPixel;  /* 1 or two bytes per greyscale pixel */

  void registerWithOpenGL();
  GLubyte *readImage( const char *filename );
  GLubyte *readRAW( const char *filename, int xdim, int ydim );

  friend class Material;

 public:

  float scale, bias;            /* scale & bias for texture-to-opengl transfer */
  int xdim, ydim;		/* texmap dimensions, in texels */
  int width, height;		/* image dimensions, in pixels */
  static bool useMipMaps;

  char *name;			/* filename */

  Texture() {}

  Texture( const char *filename ) {

    texmap = readImage( filename );
    name = strdup( filename );
    useMipMaps = true;
    scale = 1;
    bias = 0;
    if (texmap != NULL)
      registerWithOpenGL();
    else {
      width = height = xdim = ydim = 1;
      texmap = (unsigned char *) malloc( 3 );
      texmap[0] = '\0';
      texmap[1] = '\0';
      texmap[2] = '\0';
    }
  }

  Texture( char *filename, int width, int height, float s, float b ) {
    texmap = readRAW( filename, width, height );
    name = strdup( filename );
    useMipMaps = true;
    scale = s;
    bias = b;
    registerWithOpenGL();
  }

  GLuint texID() {
    return textureID;
  }

  void makeActive() {
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, textureID );
    if (hasAlpha) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else
      glDisable(GL_BLEND);
  }

  void makeInactive() {
    glDisable( GL_TEXTURE_2D );
  }

  void drawOnSquare() {
    glBegin( GL_POLYGON );

    glTexCoord2f( 0, 0 );
    glVertex3f( -1, -1, 0 );

    glTexCoord2f( width/(float)xdim, 0 );
    glVertex3f( 1, -1, 0 );

    glTexCoord2f( width/(float)xdim, height/(float)ydim );
    glVertex3f( 1, 1, 0 );

    glTexCoord2f( 0, height/(float)ydim );
    glVertex3f( -1, 1, 0 );

    glEnd();
  }

  vector texelInterp( float i, float j );
  vector texel( int i, int j, float &alpha );
  vector texel( int i, int j );
  float texelLum( int x, int y );

  Texture *findTexture( char *name );
};


float matchPixels( Texture &t1, vector &p1, Texture &t2, vector &p2, int KERNEL_HALFWIDTH );


#endif
