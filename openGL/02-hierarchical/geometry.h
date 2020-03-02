/* geometry.h
 *
 * Base class for geometry.  VAO is set up in the derived class.
 */

#ifndef GEOMETRY_H
#define GEOMETRY_H


#include "headers.h"
#include "linalg.h"


class Geometry {

 public:

  GLuint VAOindex;
  GLuint numVerts;
  GLuint primitiveType;

  Geometry() {
    VAOindex = 0;  // 0 = not set up
    primitiveType = GL_TRIANGLES; 
  }

  void draw( mat4 M, mat4 MV, mat4 MVP, vec3 lightDir, unsigned int programID ) {

    if (VAOindex != 0) {

      glUniformMatrix4fv( glGetUniformLocation( programID, "M"),          1, GL_TRUE, &MV[0][0]    );
      glUniformMatrix4fv( glGetUniformLocation( programID, "MV"),         1, GL_TRUE, &MV[0][0]    );
      glUniformMatrix4fv( glGetUniformLocation( programID, "MVP"),        1, GL_TRUE, &MVP[0][0]   );
      glUniform3fv(       glGetUniformLocation( programID, "lightDir"),   1,          &lightDir[0] );

      glBindVertexArray( VAOindex );
      glDrawArrays( primitiveType, 0, numVerts );
      glBindVertexArray( 0 );
    }
  }
};


#endif
