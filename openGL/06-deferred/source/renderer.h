// G-buffer renderer

#ifndef RENDER_H
#define RENDER_H


#include "wavefront.h"
#include "gpuProgram.h"
#include "gbuffer.h"


class Renderer {

  enum { COLOUR_GBUFFER,
	 NORMAL_GBUFFER,
	 DEPTH_GBUFFER,
	 NUM_GBUFFERS };

  GPUProgram *pass1Prog, *pass2Prog;
  GBuffer    *gbuffer;

 public:

  int debug;

  Renderer( int windowWidth, int windowHeight ) {
    gbuffer = new GBuffer( windowWidth, windowHeight, NUM_GBUFFERS );
    pass1Prog = new GPUProgram( "shaders/pass1.vert", "shaders/pass1.frag" );
    pass2Prog = new GPUProgram( "shaders/pass2.vert", "shaders/pass2.frag" );
    debug = 1;
  }

  ~Renderer() {
    delete gbuffer;
    delete pass2Prog;
    delete pass1Prog;
  }

  void reshape( int windowWidth, int windowHeight ) {
    delete gbuffer;
    gbuffer = new GBuffer( windowWidth, windowHeight, NUM_GBUFFERS );
  }

  void render( wfModel *obj, mat4 &M, mat4 &MV, mat4 &MVP, vec3 &lightDir );

  void incDebug() {
    debug = (debug+1) % 2;
  }

  void makeStatusMessage( char *buffer ) {
    if (debug == 0)
      sprintf( buffer, "Program output (after pass 2)" );
    else
      sprintf( buffer, "After pass %d", debug );
  }
};

#endif
