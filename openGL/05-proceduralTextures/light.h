// light.h


#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"

class Light {

  static int nextLight;

 public:

  float ambient[4];
  float diffuse[4];
  float specular[4];
  float position[4];

  GLenum lightID;

  Light() {
    ambient[0]  = ambient[1]  = ambient[2]  = 0.2;
    diffuse[0]  = diffuse[1]  = diffuse[2]  = 1.0;
    specular[0] = specular[1] = specular[2] = 1.0;
    position[0] = position[1] = position[2] = 1.0;
    ambient[3] = diffuse[3] = specular[3] = position[3] = 0.0;
    lightID = (GLenum) (GL_LIGHT0 + nextLight);
    nextLight++;
  }

  Light( vector v, vector c ) {
    ambient[0] = c.x * 0.1; ambient[1] = c.y * 0.1; ambient[2] = c.z * 0.2;
    diffuse[0] = c.x; diffuse[1] = c.y; diffuse[2] = c.z;
    specular[0] = c.x; specular[1] = c.y; specular[2] = c.z;
    position[0] = v.x; position[1] = v.y; position[2] = v.z;
    ambient[3] = diffuse[3] = specular[3] = position[3] = 0.0;
    lightID = (GLenum) (GL_LIGHT0 + nextLight);
    nextLight++;
  }

  void enable() {
    glLightfv( lightID, GL_AMBIENT,  &ambient[0] );
    glLightfv( lightID, GL_DIFFUSE,  &diffuse[0] );
    glLightfv( lightID, GL_SPECULAR, &specular[0] );
    glLightfv( lightID, GL_POSITION, &position[0] );
    glEnable( lightID );
  }
};


#endif
