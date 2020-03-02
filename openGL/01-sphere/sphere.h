/* sphere.h
 */


#ifndef SPHERE_H
#define SPHERE_H


#include "linalg.h"
#include "seq.h"

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

 public:

  seq<vec3> verts;

  Sphere( int numLevels ) {

    for (int i=0; i<NUM_VERTS; i++)
      verts.add( icosahedronVerts[i] );

    for (int i=0; i<verts.size(); i++)
      verts[i] = verts[i].normalize();

    for (int i=0; i<NUM_FACES; i++)
      faces.add( SphereFace( icosahedronFaces[i][0],
			     icosahedronFaces[i][1],
			     icosahedronFaces[i][2] ) );

    for (int i=0; i<numLevels; i++)
      refine();
  };

  void refine();
  void genSphere( GLfloat *&vertexBuffer, GLfloat *&normalBuffer, GLuint *&indexBuffer, int &nVerts, int &nFaces );

 private:

  seq<SphereFace> faces;

  static vec3 icosahedronVerts[NUM_VERTS];
  static int icosahedronFaces[NUM_FACES][3];
};

#endif
