/* sphere.h
 */


#ifndef SPHERE_H
#define SPHERE_H


#include "linalg.h"
#include "seq.h"
#include "gpuProgram.h"


// icosahedron vertices (taken from Jon Leech http://www.cs.unc.edu/~jon)

#define tau 0.8506508084      /* t=(1+sqrt(5))/2, tau=t/sqrt(1+t^2)  */
#define one 0.5257311121      /* one=1/sqrt(1+t^2) , unit sphere     */

#define NUM_VERTS 12
#define NUM_FACES 20


class SphereFace {
 public:
  unsigned int v[3];
  SphereFace() {}
  SphereFace( int v0, int v1, int v2 ) {
    v[0] = v0; v[1] = v1; v[2] = v2;
  }
};


class Sphere {

  seq<vec3> verts;
  seq<SphereFace> faces;

  static vec3 icosahedronVerts[NUM_VERTS];
  static int icosahedronFaces[NUM_FACES][3];

  GLuint VAO;
  GPUProgram program;
  static char *vertShader;
  static char *fragShader;

 public:

  Sphere( int numLevels );

  void refine();
  void drawGeometry( mat4 &MV, mat4 &MVP, vec3 &lightDir );
};


#endif
