// Demo of hierarchical modelling and rendering


#include "headers.h"
#include "gpuProgram.h"
#include "linalg.h"
#include "link.h"
#include "axes.h"
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

#define WINDOW_WIDTH  700
#define WINDOW_HEIGHT 700


// World objects


vector<Link *> links;
Axes *axes;
GPUProgram linkProgram;

unsigned int activeLink = 0;
float eyeDistance = 2.3;
float eyeAzimuth = -20;
int lastDigit = 0;

string shaders[] = { "link", "flat" };
int shaderIndex = 1;

bool pauseAnim = false;

void writeScene();
void readScene();


void display()

{
  vec3 eyePosition( 0, links.size() * 2.5 / 2.0, (links.size() + 1) * eyeDistance );

  // model-to-world (i.e. OCS-to-WCS)

  mat4 M = identity4();

  // model-to-world, then world-to-view (i.e. OCS-to-VCS)

  mat4 MV = translate( -1 * eyePosition ) * rotate( eyeAzimuth*M_PI/180.0, vec3(0,1,0) );

  // model-to-world, then world-to-view, then view-to-projection transform (i.e. OCS-to-CCS)

  mat4 MVP
    = perspective( 100.0*M_PI/180.0, 1.333, 0.1, 100 )
    * MV;

  // Light direction in VCS is above, to the right, and behind the
  // eye.  That's in direction (1,1,1) since the view direction is
  // down the -z axis.

  vec3 lightDir(1,1,3);
  lightDir = lightDir.normalize();

  // Draw the world

  if (links.size() > 0) {
    linkProgram.activate();
    links[0]->draw( M, MV, MVP, lightDir, linkProgram.id() ); // draws descendants, too
    linkProgram.deactivate();
  }

  axes->draw( MVP );
}


// Error callback

void errorCallback( int error, const char* description )

{
  cerr << "Error " << error << ": " << description << endl;
  exit(1);
}


// Key callback

void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
  
{
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    switch (key) {

    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose( window, GL_TRUE );
      break;

    case '+':
    case '=':
      if (activeLink < links.size()-1) {
	links[activeLink]->selected = false;
	activeLink++;
	links[activeLink]->selected = true;
      }
      break;

    case '-':
    case '_':
      if (activeLink > 0) {
	links[activeLink]->selected = false;
	activeLink--;
	links[activeLink]->selected = true;
      }
      break;

    case '<':
    case ',':
      if (activeLink < links.size())
	links[activeLink]->rotateObj(  10*M_PI/180.0, vec3(0,0,1) );
      break;

    case '>':
    case '.':
      if (activeLink < links.size())
	links[activeLink]->rotateObj( -10*M_PI/180.0, vec3(0,0,1) );
      break;

    case 'W':
      writeScene();
      break;

    case 'R':
      readScene();
      break;

    case GLFW_KEY_UP:
      eyeDistance /= 1.1;
      break;
      
    case GLFW_KEY_DOWN:
      eyeDistance *= 1.1;
      break;
      
    case GLFW_KEY_LEFT:
      eyeAzimuth -= 2.5;
      break;
      
    case GLFW_KEY_RIGHT:
      eyeAzimuth += 2.5;
      break;
    }
    
    if (key >= '0' && key <= '9')
      lastDigit = key - '0';
  }
}


// Update the world state

void updateState( float elapsedSeconds )

{
  if (pauseAnim)
    return;

  // do nothing here

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

  // Set up world

  axes = new Axes();

  linkProgram.initFromFile( (shaders[shaderIndex] + ".vert").c_str(), (shaders[shaderIndex] + ".frag").c_str() );

  readScene();

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



void writeScene()
  
{
  char filename[100];
  sprintf( filename, "scene%d.txt", lastDigit );
  ofstream out( filename );

  for (unsigned int i=0; i<links.size(); i++)
    out << *links[i] << endl;
}


void readScene()
  
{
  char filename[100];
  sprintf( filename, "scene%d.txt", lastDigit );
  ifstream in( filename );

  // Read the links

  links.clear();
  activeLink = 0;

  do {
    Link *link = new Link();
    in >> *link;
    if (in)
      links.push_back( link );
  } while (in);

  // Connect the links

  for (unsigned int i=0; i<links.size(); i++)
    for (unsigned int j=0; j< links[i]->childNames.size(); j++) {

      unsigned int k;
      for (k=0; k<links.size(); k++)
	if (links[i]->childNames[j] == links[k]->name)
	  break;

      if (k == links.size())
	cerr << "Link " << links[i]->name
	     << ": Can't find child link " << links[i]->childNames[j] << endl;
      else
	links[i]->addChild( links[k] );
    }

  if (links.size() > 0)
    links[0]->selected = true;
}
