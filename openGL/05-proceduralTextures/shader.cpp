//shader.cpp


#include "shader.h"
#include "headers.h"


static char* textFileRead(const char *fileName)

{
  char* text;
    
  if (fileName != NULL) {
    FILE *file = fopen(fileName, "rt");
        
    if (file != NULL) {
      fseek(file, 0, SEEK_END);
      int count = ftell(file);
      rewind(file);
            
      if (count > 0) {
	text = (char*)malloc(sizeof(char) * (count + 1));
	count = fread(text, sizeof(char), count, file);
	text[count] = '\0';
      }
      fclose(file);
    }
  }
  return text;
}


void glErrorReport( char *where )

{
  GLuint errnum;
  const char *errstr;

  while ((errnum = glGetError())) {
    errstr = reinterpret_cast<const char *>(gluErrorString(errnum));
    printf("%s: %s\n", where, errstr);
  }
}


static void validateShader(GLuint shader, const char* file = 0)

{
  const unsigned int BUFFER_SIZE = 512;
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  GLsizei length = 0;
    
  glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
  if (length > 0) {
    cout << "Shader " << shader << " (" << (file?file:"") << ") compile log: " << buffer << endl;
  }
}


static void validateProgram(GLuint program)

{
  const unsigned int BUFFER_SIZE = 512;
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  GLsizei length = 0;
    
  memset(buffer, 0, BUFFER_SIZE);
  glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
  if (length > 0)
    cout << "Program " << program << " link log: " << buffer << endl;
    
  glValidateProgram(program);
  GLint status;
  glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
  if (status == GL_FALSE)
    cout << "Error validating shader " << program << endl;
}


void Shader::init( const char *fsFile, const char *vsFile )

{
  glErrorReport( "before Shader::init" );

  // Fragment shader

  shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
    
  const char* fsText = textFileRead(fsFile);	
    
  if (fsText == NULL) {
    cerr << "Shader file '" << fsFile << "' not found." << endl;
    return;
  }
    
  glShaderSource( shader_fp, 1, &fsText, 0 );
    
  glCompileShader( shader_fp );
  validateShader( shader_fp, fsFile );
    
  // Vertex shader

  shader_vp = glCreateShader(GL_VERTEX_SHADER);
    
  const char* vsText = textFileRead(vsFile);	
    
  if (vsText == NULL) {
    cerr << "Shader file '" << vsFile << "' not found." << endl;
    return;
  }
    
  glShaderSource( shader_vp, 1, &vsText, 0 );
    
  glCompileShader( shader_vp );
  validateShader( shader_vp, vsFile );
    
  // GLSL program

  shader_id = glCreateProgram();
  glAttachShader( shader_id, shader_fp );
  glAttachShader( shader_id, shader_vp );
  glLinkProgram( shader_id );
  validateProgram( shader_id );

  glErrorReport( "after Shader::init" );
}
