/* sphere.C
 */


#include "sphere.h"


// icosahedron vertices (taken from Jon Leech http://www.cs.unc.edu/~jon)

vec3 Sphere::icosahedronVerts[NUM_VERTS] = {
  vec3(  tau,  one,    0 ),
  vec3( -tau,  one,    0 ),
  vec3( -tau, -one,    0 ),
  vec3(  tau, -one,    0 ),
  vec3(  one,   0 ,  tau ),
  vec3(  one,   0 , -tau ),
  vec3( -one,   0 , -tau ),
  vec3( -one,   0 ,  tau ),
  vec3(   0 ,  tau,  one ),
  vec3(   0 , -tau,  one ),
  vec3(   0 , -tau, -one ),
  vec3(   0 ,  tau, -one )
};


// icosahedron faces (taken from Jon Leech http://www.cs.unc.edu/~jon)

int Sphere::icosahedronFaces[NUM_FACES][3] = {
  { 4, 8, 7 },
  { 4, 7, 9 },
  { 5, 6, 11 },
  { 5, 10, 6 },
  { 0, 4, 3 },
  { 0, 3, 5 },
  { 2, 7, 1 },
  { 2, 1, 6 },
  { 8, 0, 11 },
  { 8, 11, 1 },
  { 9, 10, 3 },
  { 9, 2, 10 },
  { 8, 4, 0 },
  { 11, 0, 5 },
  { 4, 9, 3 },
  { 5, 3, 10 },
  { 7, 8, 1 },
  { 6, 1, 11 },
  { 7, 2, 9 },
  { 6, 10, 2 },
};


void Sphere::refine()

{
  int n = faces.size();

  for (int i=0; i<n; i++) {

    SphereFace f = faces[i];

    verts.add( (verts[ f.v[0] ] + verts[ f.v[1] ]).normalize() );
    verts.add( (verts[ f.v[1] ] + verts[ f.v[2] ]).normalize() );
    verts.add( (verts[ f.v[2] ] + verts[ f.v[0] ]).normalize() );

    int v01 = verts.size() - 3;
    int v12 = verts.size() - 2;
    int v20 = verts.size() - 1;

    faces.add( SphereFace( f.v[0], v01, v20 ) );
    faces.add( SphereFace( f.v[1], v12, v01 ) );
    faces.add( SphereFace( f.v[2], v20, v12 ) );

    faces[i].v[0] = v01;
    faces[i].v[1] = v12;
    faces[i].v[2] = v20;
  }
}


void Sphere::genSphere( GLfloat *&vertexBuffer, GLfloat *&normalBuffer, GLuint *&indexBuffer, int &nVerts, int &nFaces )

{
  // copy vertices

  nVerts = verts.size();

  vertexBuffer = new GLfloat[ nVerts * 3 ];

  for (int i=0; i<nVerts; i++)
    ((vec3 *) vertexBuffer)[i] = verts[i];

  // copy normals
  //
  // Since the vertices are on a sphere centred at the origin, and are
  // distance 1 from the origin, the normals are the same as the
  // vertices.

  normalBuffer = new GLfloat[ nVerts * 3 ];

  for (int i=0; i<nVerts; i++)
    ((vec3 *) normalBuffer)[i] = verts[i];

  // copy faces

  nFaces = faces.size();

  indexBuffer = new GLuint[ nFaces * 3 ];

  for (int i=0; i<faces.size(); i++)
    for (int j=0; j<3; j++) 
      indexBuffer[3*i+j] = faces[i].v[j]; 
}
