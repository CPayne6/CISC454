// main.cpp


#include "headers.h"
#include "persp.h"


perspWindow *w;


void idle()

{
  w->idle();
}


int main( int argc, char **argv )

{
  ilInit();

  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
  //  glutIdleFunc( idle );

  w = new perspWindow( argc, argv, 0, 0, 800, 600, "GLSL simple texture example" );

  glewInit(); // must do this *after* window is created!  Why?
  if (!glewIsSupported("GL_VERSION_2_0")) {
    cerr << "OpenGL 2.0 is not supported" << endl;
    exit( 1 );
  }

  glutMainLoop();

  return 0;
}
