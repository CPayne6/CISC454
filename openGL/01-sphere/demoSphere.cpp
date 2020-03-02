// OpenGL demoSphere
//
// Draw a sphere with vertex normals


#include "headers.h"
#include "gpuProgram.h"
#include "linalg.h"
#include "sphere.h"
#include "axes.h"

#define WINDOW_WIDTH  700
#define WINDOW_HEIGHT 700

#define ROTATION_RATE (45.0/180.0*3.14159) // 45 degrees per second, but represented in radians per second


GLuint myVAO;

GPUProgram *myGPUProgram;	// pointer to GPU program object

int numSphereFaces;

Axes *axes;			// x,y,z axes

bool pauseAnim = false;

float theta = 0;
float eyeDistance = 4;


// Error callback

void errorCallback( int error, const char* description )

{
  cerr << "Error " << error << ": " << description << endl;
  exit(1);
}


// Key callback

void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
  
{
  if (action == GLFW_PRESS)
    switch (key) {
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose( window, GL_TRUE );
      break;
    case 'P': // Note that GLFW always returns the uppercase letter, even if the lowercase letter was pressed.
      pauseAnim = !pauseAnim;
      break;
    case GLFW_KEY_UP:
      eyeDistance /= 1.1;
      break;
    case GLFW_KEY_DOWN:
      eyeDistance *= 1.1;
      break;
  }
}

  
void setupVAO()

{
  // build a set of icosahedron vertices 

  GLfloat *verts;
  GLfloat *norms;
  GLuint  *faces;

  int nVerts, nFaces;

  Sphere s( 1 ); // argument is number of times icosahedron gets refined (i.e. made finer)

  s.genSphere( verts, norms, faces, nVerts, nFaces );
  
  // Create a VAO for the cube

  glGenVertexArrays( 1, &myVAO );
  glBindVertexArray( myVAO );

  // store vertices (i.e. one triple of floats per vertex)

  GLuint vertexbuffer;
  glGenBuffers( 1, &vertexbuffer );
  glBindBuffer( GL_ARRAY_BUFFER, vertexbuffer );

  glBufferData( GL_ARRAY_BUFFER, nVerts * 3 * sizeof(GLfloat), verts, GL_STATIC_DRAW );

  // attribute 0 = position

  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

  // store vertex normals (i.e. one triple of floats per vertex)

  GLuint normalbuffer;
  glGenBuffers( 1, &normalbuffer );
  glBindBuffer( GL_ARRAY_BUFFER, normalbuffer );

  glBufferData( GL_ARRAY_BUFFER, nVerts * 3 * sizeof(GLfloat), norms, GL_STATIC_DRAW );

  // attribute 1 = normal

  glEnableVertexAttribArray( 1 );
  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );

  // store faces (i.e. one triple of vertex indices per face)

  GLuint indexBuffer;
  glGenBuffers( 1, &indexBuffer );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, nFaces * 3 * sizeof(GLuint), faces, GL_STATIC_DRAW );

  // store nFaces in a global for later use when displaying

  numSphereFaces = nFaces;
}


// Drawing function


void display()

{
  glClearColor( 0.0, 0.0, 0.0, 0.0 );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glEnable( GL_DEPTH_TEST );

  myGPUProgram->activate();

  // model-view transform (i.e. OCS-to-VCS)

  vec3 eyePosition( 0, 0, eyeDistance );

  mat4 MV
    = translate( -1 * eyePosition )		       // V
    * rotate( theta, vec3( 1, 1, 1 ) );		       // M

  glUniformMatrix4fv( glGetUniformLocation( myGPUProgram->id(), "MV"), 1, GL_TRUE, &MV[0][0] );

  // model-view-projection transform (i.e. OCS-to-CCS)

  mat4 MVP
    = perspective( 100.0*M_PI/180.0, WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1, 100 ) // P
    * MV;

  glUniformMatrix4fv( glGetUniformLocation( myGPUProgram->id(), "MVP"), 1, GL_TRUE, &MVP[0][0] );

  // Light direction in VCS is above, to the right, and behind the
  // eye.  That's in direction (1,1,1) since the view direction is
  // down the -z axis.

  vec3 lightDir(1,1,1);
  lightDir.normalize();

  glUniform3fv( glGetUniformLocation( myGPUProgram->id(), "lightDir"), 1, &lightDir[0] );

  // Draw using element array

  glBindVertexArray( myVAO );
  glDrawElements( GL_TRIANGLES, numSphereFaces*3, GL_UNSIGNED_INT, 0 );

  myGPUProgram->deactivate();

  // Draw the axes

  axes->draw( MVP );
}



// Update the world state

void updateState( float elapsedSeconds )

{
  if (pauseAnim)
    return;

  // increment angle

  theta += ROTATION_RATE * elapsedSeconds;
}




// Main program


int main( int argc, char **argv )

{
  // Trap all errors (do this *before* creating the window)

  glfwSetErrorCallback( errorCallback );
  
  // Init everything

  GLFWwindow* window;

  if (!glfwInit())
    return 1;
  
  glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );

  window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Demo Sphere", NULL, NULL);
  
  if (!window) {
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent( window );
  glfwSwapInterval( 1 );
  gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress );

  // Define a key handler (do this *after* creating the window)

  glfwSetKeyCallback( window, keyCallback );

  // Shaders for sphere

  myGPUProgram = new GPUProgram( "demoSphere.vert", "demoSphere.frag" );
  axes = new Axes();

  // Sphere

  setupVAO();

  // Main loop

  struct timeb prevTime, thisTime; // record the last rendering time
  ftime( &prevTime );

  glEnable( GL_DEPTH_TEST );

  while (!glfwWindowShouldClose( window )) {

    // Find elapsed time since last render

    ftime( &thisTime );
    float elapsedSeconds = (thisTime.time + thisTime.millitm / 1000.0) - (prevTime.time + prevTime.millitm / 1000.0);
    prevTime = thisTime;

    // Update the world state

    updateState( elapsedSeconds );

    // Clear, display, and check for events

    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear depth buffer

    display();

    glfwSwapBuffers( window );
    glfwPollEvents();
  }

  // Clean up

  glfwDestroyWindow( window );
  glfwTerminate();

  return 0;
}



