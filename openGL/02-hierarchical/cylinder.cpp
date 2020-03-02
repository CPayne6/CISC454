/* cylinder.cpp
 *
 * Set up a VAO for a cylinder.  The cylinder is aligned with the z
 * axis, has diameter 1, and goes from z=-0.5 to z=0.5.  The 'transform' is
 * applied to initially pose the cylinder.
 */


#include "cylinder.h"


void Cylinder::create( mat4 M )

{
  // Create a VAO

  glGenVertexArrays( 1, &VAOindex ); // VAOindex is defined in base Geometry class
  glBindVertexArray( VAOindex );

  numVerts = 12 * numSlices;

  vec3 verts[ numVerts ];	// positions
  vec3 norms[ numVerts ];	// normals

  // Store positions and normals slice-by-slice

  for (int i=0; i<numSlices; i++) {

    float sinCCW = 0.5 * sin( (float)  i   / (float)numSlices * 2 * M_PI );
    float cosCCW = 0.5 * cos( (float)  i   / (float)numSlices * 2 * M_PI );

    float sinCW  = 0.5 * sin( (float)(i+1) / (float)numSlices * 2 * M_PI );
    float cosCW  = 0.5 * cos( (float)(i+1) / (float)numSlices * 2 * M_PI );

    vec3* v = &verts[12*i];

    // -- POSITIONS --

    // top triangle i

    *v++ = (M * vec4( 0,      0,       0.5, 1 )).toVec3();
    *v++ = (M * vec4( cosCW,  sinCW,   0.5, 1 )).toVec3();
    *v++ = (M * vec4( cosCCW, sinCCW,  0.5, 1 )).toVec3();

    // bottom triangle i

    *v++ = (M * vec4( 0,      0,      -0.5, 1 )).toVec3();
    *v++ = (M * vec4( cosCCW, sinCCW, -0.5, 1 )).toVec3();
    *v++ = (M * vec4( cosCW,  sinCW,  -0.5, 1 )).toVec3();

    // panel triangles

    *v++ = (M * vec4( cosCCW, sinCCW,  0.5, 1 )).toVec3();
    *v++ = (M * vec4( cosCCW, sinCCW, -0.5, 1 )).toVec3();
    *v++ = (M * vec4( cosCW,  sinCW,   0.5, 1 )).toVec3();

    *v++ = (M * vec4( cosCW,  sinCW,   0.5, 1 )).toVec3();
    *v++ = (M * vec4( cosCCW, sinCCW, -0.5, 1 )).toVec3();
    *v++ = (M * vec4( cosCW,  sinCW,  -0.5, 1 )).toVec3();

    // -- NORMALS --

    vec3* n = &norms[12*i];

    // top triangle i

    *n++ = (M * vec4( 0, 0, 1, 0 )).toVec3();
    *n++ = (M * vec4( 0, 0, 1, 0 )).toVec3();
    *n++ = (M * vec4( 0, 0, 1, 0 )).toVec3();

    // bottom triangle i

    *n++ = (M * vec4( 0, 0, -1, 0 )).toVec3();
    *n++ = (M * vec4( 0, 0, -1, 0 )).toVec3();
    *n++ = (M * vec4( 0, 0, -1, 0 )).toVec3();

    // panel triangles

    *n++ = (M * vec4( cosCCW, sinCCW, 0, 0 )).toVec3();
    *n++ = (M * vec4( cosCCW, sinCCW, 0, 0 )).toVec3();
    *n++ = (M * vec4( cosCW,  sinCW,  0, 0 )).toVec3();

    *n++ = (M * vec4( cosCW,  sinCW,  0, 0 )).toVec3();
    *n++ = (M * vec4( cosCCW, sinCCW, 0, 0 )).toVec3();
    *n++ = (M * vec4( cosCW,  sinCW,  0, 0 )).toVec3();
  }

  // Create the position VBO
  
  GLuint VBOindex;
  glGenBuffers(1, &VBOindex);
  glBindBuffer(GL_ARRAY_BUFFER, VBOindex);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), (GLfloat*) &verts[0], GL_STATIC_DRAW);

  // Define the position attribute
  
  glEnableVertexAttribArray(0);	// 0 = position
  glBindBuffer(GL_ARRAY_BUFFER, VBOindex);
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

  // Create the normal VBO
  
  glGenBuffers(1, &VBOindex);
  glBindBuffer(GL_ARRAY_BUFFER, VBOindex);
  glBufferData(GL_ARRAY_BUFFER, sizeof(norms), (GLfloat*) &norms[0], GL_STATIC_DRAW);

  // Define the normal attribute
  
  glEnableVertexAttribArray(1);	// 1 = normal
  glBindBuffer(GL_ARRAY_BUFFER, VBOindex);
  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );

  // Done

  glBindVertexArray( 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

