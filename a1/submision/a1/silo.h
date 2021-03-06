// A Silo


#ifndef SILO_H
#define SILO_H

#include "headers.h"
#include "gpuProgram.h"
#include "buildings.h"


class Silo : public Building {
 public:

  // Constructors 

  Silo() {}

  Silo( vec3 pos ) : Building( pos ) {
    roundsLeft = 30;
    changeBarrel(pos + vec3(0,0.5,0));
  }

  bool canShoot() {
    return (roundsLeft > 0);
  }

  void decrMissiles() {
    roundsLeft--;
  }

  vec3 position() {
    return pos;
  }

  void changeBarrel(vec3 newTarget){
    vec3 dist = (newTarget - pos);
    barrelAim =  pos + 0.075 * dist.normalize();
  }

  // Draw the silo

  void draw( GPUProgram *gpuProgram ) {

    const int NUM_SEGMENTS = 45; // number of pieces of hemisphere perimeter

    vec3 *verts = new vec3[NUM_SEGMENTS+1];

    verts[0] = pos;
    
    for (int i=0; i<NUM_SEGMENTS; i++) {
      float theta = (i/(float)(NUM_SEGMENTS-1)) * M_PI;
      verts[i+1] = vec3( pos.x + 0.04 * cos(theta), pos.y + 0.04 * sin(theta), 0 );
    }

    vec3 verts2[2] = {
      pos,barrelAim
    };

    gpuProgram->drawVertices( &verts2[0], 3, GL_LINES, vec3(1,1,1) );
    gpuProgram->drawVertices( verts, NUM_SEGMENTS+1, GL_TRIANGLE_FAN, vec3(1,1,1) );

    delete [] verts;
  }

 private:

  int roundsLeft;
  vec3 barrelAim;
};
  

#endif
