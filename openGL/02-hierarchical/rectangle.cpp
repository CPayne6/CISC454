/* rectangle.cpp
 *
 * Set up a VAO for a rectangle.  * The rectangle lies in the x-y
 * plane: [-0.5,0.5] in x and [-0.5,0.5] in y.  The normals point in
 * the +z direction.  The 'transform' is applied to initially pose the
 * rectangle.
 */


#include "rectangle.h"


void Rectangle::create( mat4 M )

{
  // Create a VAO

  glGenVertexArrays( 1, &VAOindex ); // VAOindex is defined in base Geometry class
  glBindVertexArray( VAOindex );

  numVerts = 4;

  // positions

  vec3 verts[] = {
    (M * vec4( -0.5, -0.5, 0, 1 )).toVec3(),
    (M * vec4(  0.5, -0.5, 0, 1 )).toVec3(),
    (M * vec4(  0.5,  0.5, 0, 1 )).toVec3(),
    (M * vec4( -0.5,  0.5, 0, 1 )).toVec3()
  };

  // normals

  vec3 norms[] = {
    (M * vec4( 0, 0, 1, 0 )).toVec3().normalize(), 
    (M * vec4( 0, 0, 1, 0 )).toVec3().normalize(), 
    (M * vec4( 0, 0, 1, 0 )).toVec3().normalize(), 
    (M * vec4( 0, 0, 1, 0 )).toVec3().normalize()
  };

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

