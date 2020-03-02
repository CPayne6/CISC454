// persp.h

#ifndef PERSP_H
#define PERSP_H

#include "headers.h"
#include "arcballWindow.h"
#include "vector.h"
#include "light.h"
#include "shader.h"
#include "texture.h"


const int numRenderModes = 6;

#define NUM_TEXTURES 2

class perspWindow : public arcballWindow {

  Light light0;
  float angle;
  Shader *shader;
  Texture *texture[NUM_TEXTURES];
  int renderMode;
  int currentTexture;
  float perlinLevels;
  float captureRadius;
  static char *modeString[];

 public:

 perspWindow( int argc, char **argv, int x, int y, int width, int height, char *title )
   : arcballWindow( x, y, width, height, title )

    {
      ReadViewParams( ".view0" );
      light0 = Light( vector(1,1,1), vector(1,1,1) );
      angle = 0;
      shader = NULL;
      renderMode = 0;
      currentTexture = 0;
      perlinLevels = 4;
      captureRadius = 1;
      for (int i=0; i<NUM_TEXTURES; i++) {
	char fn[100];
	sprintf( fn, "perlin%d.png", i );
	texture[i] = new Texture( fn );
      }
    }

  void idle() {
    angle += 0.1;
    glutPostRedisplay();
  }

  void draw();
  void userKeyAction( unsigned char key, int x, int y );
  void userMouseClick( vector v ) {};
  bool userMouseAction( int button, int state, int x, int y ) { return false; };
  bool userMouseMotion( int x, int y ) { return false; };
};


#endif
