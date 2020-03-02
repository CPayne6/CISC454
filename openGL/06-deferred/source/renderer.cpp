// G-buffer renderer


#include "headers.h"
#include "renderer.h"
#include "shader.h"


// Draw a quad over the full screen.  This generates a fragment for
// each pixel on the screen, allowing the fragment shader to run on
// each fragment.


void drawFullscreenQuad()

{
  vec2 verts[4] = { vec2( -1, -1 ), vec2( -1, 1 ), vec2( 1, -1 ), vec2( 1, 1 ) };
  vec2 texcoords[4] = { vec2( 0, 0 ), vec2( 0, 1 ), vec2( 1, 0 ), vec2( 1, 1 ) };
    
  GLuint VAO, VBO[2];

  glGenVertexArrays( 1, &VAO );
  glBindVertexArray( VAO );

  glGenBuffers( 2, VBO );

  glBindBuffer( GL_ARRAY_BUFFER, VBO[0] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW );
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, 0 );

  glBindBuffer( GL_ARRAY_BUFFER, VBO[1] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW );
  glEnableVertexAttribArray( 1 );
  glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, 0 );

  glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

  glDeleteBuffers( 2, VBO );
  glDeleteVertexArrays( 1, &VAO );
}


// Render the scene in two passes.


void Renderer::render( wfModel *obj, mat4 &M, mat4 &MV, mat4 &MVP, vec3 &lightDir )

{
  // Pass 1: Store colour, normal, depth in G-Buffers

  gbuffer->BindForWriting();

  pass1Prog->activate();

  pass1Prog->setMat4(  "M",   M );
  pass1Prog->setMat4(  "MV",  MV );
  pass1Prog->setMat4(  "MVP", MVP );

  gbuffer->BindTexture( COLOUR_GBUFFER );
  gbuffer->BindTexture( NORMAL_GBUFFER );
  gbuffer->BindTexture( DEPTH_GBUFFER  );

  int activeDrawBuffers1[] = { COLOUR_GBUFFER, NORMAL_GBUFFER, DEPTH_GBUFFER };
  gbuffer->setDrawBuffers( 3, activeDrawBuffers1 );

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glEnable( GL_DEPTH_TEST );

  obj->draw( pass1Prog );

  pass1Prog->deactivate();

  if (debug == 1) {
    gbuffer->DrawGBuffers();
    return;
  }

  // Pass 2: Draw everything using data from G-Buffers

  glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 ); // default framebuffer ... not an FBO
  glClear( GL_COLOR_BUFFER_BIT );
  glDisable( GL_DEPTH_TEST );

  pass2Prog->activate();

  gbuffer->BindTexture( COLOUR_GBUFFER );
  gbuffer->BindTexture( NORMAL_GBUFFER );
  gbuffer->BindTexture( DEPTH_GBUFFER );

  pass2Prog->setInt(  "colourSampler", COLOUR_GBUFFER );
  pass2Prog->setInt(  "normalSampler", NORMAL_GBUFFER );
  pass2Prog->setInt(  "depthSampler",  DEPTH_GBUFFER );
  pass2Prog->setVec3( "lightDir",      lightDir );

  drawFullscreenQuad();

  pass2Prog->deactivate();
}
