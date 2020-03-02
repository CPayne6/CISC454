/* sphere.cpp
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


Sphere::Sphere( int numLevels )

{
  // Create the sphere

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

  // Set up the VAO

  glGenVertexArrays( 1, &VAO );
  glBindVertexArray( VAO );

  GLuint bufferID;

  // store vertices (attribute 0)

  int nVerts = verts.size();

  GLfloat vertices[ nVerts * 3 ];

  for (int i=0; i<nVerts; i++)
    ((vec3 *) vertices)[i] = verts[i];

  glGenBuffers( 1, &bufferID );
  glBindBuffer( GL_ARRAY_BUFFER, bufferID );
  glBufferData( GL_ARRAY_BUFFER, nVerts * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW );
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

  // store vertex normals (attribute 1)

  GLfloat normals[ nVerts * 3 ];

  for (int i=0; i<nVerts; i++)
    ((vec3 *) normals)[i] = verts[i];

  glGenBuffers( 1, &bufferID );
  glBindBuffer( GL_ARRAY_BUFFER, bufferID );
  glBufferData( GL_ARRAY_BUFFER, nVerts * 3 * sizeof(GLfloat), normals, GL_STATIC_DRAW );
  glEnableVertexAttribArray( 1 );
  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );

  // store faces (i.e. one triple of vertex indices per face)

  int nFaces = faces.size();

  GLuint indices[ nFaces * 3 ];

  for (int i=0; i<faces.size(); i++)
    for (int j=0; j<3; j++) 
      indices[3*i+j] = faces[i].v[j]; 

  glGenBuffers( 1, &bufferID );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferID );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, nFaces * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW );

  // shaders

  program.initFromFile( "sphere.vert", "sphere.frag" );
}


void Sphere::drawGeometry( mat4 &MV, mat4 &MVP, vec3 &lightDir )

{
  program.activate();

  glUniformMatrix4fv( glGetUniformLocation( program.id(), "MV"),       1, GL_TRUE, &MV[0][0]    );
  glUniformMatrix4fv( glGetUniformLocation( program.id(), "MVP"),      1, GL_TRUE, &MVP[0][0]   );
  glUniform3fv(       glGetUniformLocation( program.id(), "lightDir"), 1,          &lightDir[0] );

  glBindVertexArray( VAO );
  glDrawElements( GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0 );

  program.deactivate();
}



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


