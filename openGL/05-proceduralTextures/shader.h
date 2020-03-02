// shader.h


#ifndef SHADER_H
#define SHADER_H

#include "headers.h"
#include <string>

class Shader {

  unsigned int shader_id;
  unsigned int shader_fp;
  unsigned int shader_vp;

 public:

  Shader() {};

  Shader( const char *fsFile, const char *vsFile ) {
    init( fsFile, vsFile );
  }

  ~Shader() {
    glDetachShader( shader_id, shader_fp );
    glDeleteShader( shader_fp );

    glDetachShader( shader_id, shader_vp );
    glDeleteShader( shader_vp );

    glDeleteProgram( shader_id );
  }

  void init( const char *fsFile, const char *vsFile );

  int id() {
    return shader_id;
  }

  void makeActive() {
    glUseProgram( shader_id );
  }

  void makeInactive() {
    glUseProgram( 0 );
  }

};

#endif
