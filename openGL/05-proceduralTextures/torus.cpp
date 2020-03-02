// torus.cpp
//
// Draw a torus in the xy plane with major radius 'majorR' and minor
// radius 'minorR'.  Include texture coordinates in [0,1]x[0,1] with
// the major coordinate first and the minor coordinate second.


#include "headers.h"
#include "vector.h"


void drawTorus( float majorR, float minorR )

{
  vector z( 0, 0, 1 );
  
  float majorInc = M_PI/60.0;
  float minorInc = M_PI/30.0;

  glBegin( GL_QUAD_STRIP );

  for (float major=0; major<2*M_PI+majorInc/2; major += majorInc) {

    vector x0( cos(major),              sin(major),              0 );
    vector x1( cos(major+majorInc),     sin(major+majorInc),     0 );

    vector o0 = majorR * x0;
    vector o1 = majorR * x1;

    for (float minor=0; minor<2*M_PI+minorInc/2; minor += minorInc) {

      // Draw quad at (major,minor)

      vector n00 = cos(minor)*x0          + sin(minor)*z;
      vector n01 = cos(minor)*x1          + sin(minor)*z;
      
      vector v00 = o0 + minorR * n00;
      vector v01 = o1 + minorR * n01;

      // Output one quad

      glTexCoord2f( major/(2*M_PI), minor/(2*M_PI) );
      glNormal3fv( &n00.x );
      glVertex3fv( &v00.x );

      glTexCoord2f( (major+majorInc)/(2*M_PI), minor/(2*M_PI) );
      glNormal3fv( &n01.x );
      glVertex3fv( &v01.x );
    }
  }

  glEnd();
}    


