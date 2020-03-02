// OpenGL demo 4
//
// A square defined using a more efficient VAO.
//
// Note that this square does not yet rotate, even though there's a
// world state below for doing the rotation.  The rotation will be
// done in demo5.


#include "headers.h"
#include "gpuProgram.h"


GPUProgram *myGPUProgram;

GLuint squareVAO; // A VAO for the square


#define ROTATION_RATE (45.0/180.0*3.14159) // 45 degrees per second, but represented in radians per second


// The world state

struct tag_worldState {

  float len;    // side length
  float theta;  // angle in radians
  
} state = { 0.5, 0.0 };


// Error callback

void errorCallback( int error, const char* description )

{
  cerr << "Error " << error << ": " << description << endl;
  exit(1);
}


// Key callback

void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
  
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // exit on ESC
    glfwSetWindowShouldClose( window, GL_TRUE );
}

  
// Set up a VAO for the square

void setupSquareVAO()

{
  // Allocate space for a Vertex Array Object (VAO)

  glGenVertexArrays( 1, &squareVAO );

  // Tell OpenGL to use this VAO as the *current* Vertex Array Object
  // (i.e. for the rest of this function).

  glBindVertexArray( squareVAO );

  // Set up a VBO for the square
  //
  // The VBO stores the *generic* square which is later transformed.

  vec3 verts[6];

  verts[0] = vec3( 0, 0, 0 ); // first triangle
  verts[1] = vec3( 1, 0, 0 );
  verts[2] = vec3( 0, 1, 0 );

  verts[3] = vec3( 0, 1, 0 ); // second triangle
  verts[4] = vec3( 1, 0, 0 );
  verts[5] = vec3( 1, 1, 0 ); 

  GLuint VBO;
  glGenBuffers( 1, &VBO );
  glBindBuffer( GL_ARRAY_BUFFER, VBO );
  glBufferData( GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

  // Enable attribute 0 so that the vertex shader has access to this
  // attribute (i.e. the position).  This state is stored in the VAO.

  glEnableVertexAttribArray( 0 );

  // Unbind everything, but DO NOT delete the VBO buffer (which
  // resides on the GPU).

  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindVertexArray( 0 );
}


// Drawing function
//
// Draw the static square that is defined in the squareVAO


void display()

{
  // Use the VAO that was set up above

  glBindVertexArray( squareVAO );

  // Draw it as two triangles with six vertices
  
  glDrawArrays( GL_TRIANGLES, 0, 6 );

  // Unbind the VAO
  
  glBindVertexArray( 0 );
}



// Update the world state

void updateState( float elapsedSeconds )

{
  // increment angle

  state.theta += ROTATION_RATE * elapsedSeconds;

  // report angle (in degrees, on a single line)

  cout << '\r' << state.theta/3.14159*180 << "   ";
  cout.flush();
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

  window = glfwCreateWindow( 800, 600, "Demo 4", NULL, NULL);
  
  if (!window) {
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent( window );
  glfwSwapInterval( 1 );
  gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress );

  // Define a key handler (do this *after* creating the window)

  glfwSetKeyCallback( window, keyCallback );

  // Set up vertex and fragment shaders

  myGPUProgram = new GPUProgram( "demo2.vert", "demo2.frag" );
  myGPUProgram->activate();

  // Create the VAO for the generic square

  setupSquareVAO();

  // Main loop

  struct timeb prevTime, thisTime; // record the last rendering time
  ftime( &prevTime );

  while (!glfwWindowShouldClose( window )) {

    // Find elapsed time since last render

    ftime( &thisTime );
    float elapsedSeconds = (thisTime.time + thisTime.millitm / 1000.0) - (prevTime.time + prevTime.millitm / 1000.0);
    prevTime = thisTime;

    // Update the world state

    updateState( elapsedSeconds );

    // Clear, display, and check for events

    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glClear( GL_COLOR_BUFFER_BIT );
    display();
    glfwSwapBuffers( window );
    glfwPollEvents();
  }

  // Clean up

  glfwDestroyWindow( window );
  glfwTerminate();

  return 0;
}


