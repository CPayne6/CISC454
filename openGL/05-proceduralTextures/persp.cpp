// persp.cpp


#include "headers.h"
#include "persp.h"
#include "torus.h"
#include "font.h"


int Light::nextLight = 0;


void drawCoordSystem();


char *perspWindow::modeString[] = {
  "texture coordinates",
  "noise texture",
  "sin(2 pi x)",
  "sin(100 x)",
  "sin(100 (x + perlin(x)))",
  "sin(100 (x + perlin(x))) ^ 3"
};


void perspWindow::draw()

{
  // Lighting

  glMatrixMode( GL_MODELVIEW );
  glEnable( GL_LIGHTING );
  glLoadIdentity();
  light0.enable();

  // Setup for drawing

  glClearColor( 1,1,1,0 );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  setupProjection();

  // Enable shaders and textures

  if (shader == NULL)
    shader = new Shader( "shader.frag", "shader.vert" ); // Can only do this after window is set up

  texture[currentTexture]->makeActive();

  shader->makeActive();

  glUniform1i( glGetUniformLocation( shader->id(), "noise" ), 0 ); // "noise" is texture 0
  glUniform1i( glGetUniformLocation( shader->id(), "renderMode" ), renderMode );
  glUniform1f( glGetUniformLocation( shader->id(), "perlinLevels" ), perlinLevels );
  glUniform1f( glGetUniformLocation( shader->id(), "captureRadius" ), captureRadius );

  // Draw the scene

  glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
  glEnable( GL_COLOR_MATERIAL );

  glShadeModel( GL_SMOOTH );

  glColor3f( 0.5, 0.9, 0.6 );
  drawTorus( 5, 2 );

  // Disable shaders and textures

  texture[currentTexture]->makeInactive();

  shader->makeInactive();

  // Draw axes

  drawCoordSystem();

  // Print rendering mode in window

  char status[1000];

  sprintf( status, " %g levels | radius %-5.3g | texture %d | %s", perlinLevels, captureRadius, currentTexture, modeString[renderMode] );
  glColor3f( 0, 0, 0 );
  printString( status, 10, 10, xdim, ydim );
}


void perspWindow::userKeyAction( unsigned char key, int x, int y )

{
  static int currentView = 0;
  char name[8];

  switch (key) {

  case 27:			// quit
    exit(0);
    
  case 'w':			// write current view
    sprintf( name, ".view%d", currentView );
    OutputViewParams( name );
    break;
    
  case 'r':			// read current view
    sprintf( name, ".view%d", currentView );
    ReadViewParams( name );
    break;

  case '1':			// record a digit (for views)
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  case '0':
    currentView = (int) (key - '0');
    break;

  case '+':
  case '=':
    renderMode++;
    if (renderMode >= numRenderModes)
      renderMode = 0;
    break;

  case '-':
  case '_':
    renderMode--;
    if (renderMode < 0)
      renderMode = numRenderModes-1;
    break;

  case 't':
    currentTexture = (currentTexture+1) % NUM_TEXTURES;
    break;

  case '[':
    if (perlinLevels > 0)
      perlinLevels--;
    break;

  case ']':
    perlinLevels++;
    break;

  case '<':
  case ',':
    if (captureRadius > 0)
      captureRadius -= 0.005;
    break;

  case '>':
  case '.':
    captureRadius += 0.005;
    break;

  case '?':
    cout << "+   next render mode" << endl
	 << "-   previous render mode" << endl
	 << "t   change perlin noise texture" << endl
	 << "[   decrease number of perlin levels" << endl 
	 << "]   increase number of perlin levels" << endl
	 << "<   decrease capture radius" << endl
	 << ">   increase capture radius" << endl
	 << "r - read view N (where N = last digit pressed)" << endl
	 << "w - write view N (where N = last digit pressed)" << endl;
    break;
  }

  glutPostRedisplay();
}




// Some OpenGL functions


// Draw an arrow up the z axis.  Length is about one unit.

void drawArrow() 

{
  static GLUquadric *quadric = gluNewQuadric();

  glPushMatrix();
  glScalef( 2, 2, 2 );
  gluCylinder( quadric, 0.04, 0.04, 1, 20, 20 );
  glTranslatef( 0, 0, 1 );
  gluCylinder( quadric, 0.08, 0, 0.4, 20, 20 );
  gluQuadricOrientation( quadric, GLU_INSIDE );
  gluDisk( quadric, 0, 0.08, 20, 1 );
  gluQuadricOrientation( quadric, GLU_OUTSIDE );
  glPopMatrix();
}


// Draw a coordinate system in R,G,B for x,y,z

void drawCoordSystem()

{
  static GLUquadric *quadric = gluNewQuadric();

  glEnable( GL_NORMALIZE );

  glPushMatrix();
  glColor3f( 0.9, 0.9, 0.9 );	// origin
  glRotatef( 90, 1, 0, 0 );
  gluSphere( quadric, 0.08, 20, 20 );
  glPopMatrix();

  glPushMatrix();
  glColor3f( 0.9, 0.2, 0.2 );	// x
  glRotatef( 90, 0, 1, 0 );
  drawArrow();
  glPopMatrix();

  glPushMatrix();
  glColor3f( 0.2, 0.9, 0.2 );	// y
  glRotatef( 90, -1, 0, 0 );
  drawArrow();
  glPopMatrix();

  glPushMatrix();
  glColor3f( 0.2, 0.2, 0.9 );	// z
  drawArrow();
  glPopMatrix();

  glDisable( GL_NORMALIZE );
}
