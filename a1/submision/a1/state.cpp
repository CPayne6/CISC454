// state.cpp


#include "headers.h"

#include "state.h"


// Draw each of the objects in the world

float PROB_OF_LAUNCH = 0.99;


void State::draw() 

{
  int i;

  gpuProgram->activate();

  glUniformMatrix4fv( glGetUniformLocation( gpuProgram->id(), "M"), 1, GL_TRUE, &M[0][0] );
  
  for (i=0; i<silos.size(); i++)
    silos[i].draw( gpuProgram );

  for (i=0; i<cities.size(); i++)
    cities[i].draw( gpuProgram );

  for (i=0; i<missilesIn.size(); i++)
    missilesIn[i].draw( gpuProgram );

  for (i=0; i<missilesOut.size(); i++)
    missilesOut[i].draw( gpuProgram );

  for (i=0; i<explosions.size(); i++)
    explosions[i].draw( gpuProgram );

  gpuProgram->deactivate();
}

// Update the state of the world after some time interval, deltaT
//
// CHANGE ALL OF THIS FUNCTION


void State::updateState( float deltaT )

{
  
  int i;

  // Update the time

  currentTime += deltaT;

  // Generate some new missiles.  The rate of missile generation
  // should increase with time.
  //
  // CHANGE THIS

  if (randIn01() > PROB_OF_LAUNCH) {	// New missile 
    
    float sourceX = randIn01();
    vec3 source = vec3( sourceX, worldTop, 0);
    float v = (randIn01()-0.5)/(float)20;
    if((sourceX < 0.2 && v < 0) || (sourceX > 0.8 && v > 0)){
        v = v*-1;
    }
    missilesIn.add( Missile( source, // source
			     (vec3( v, -0.1, 0 )),         // velocity
			     0,                              // destination y
			     vec3( 1,1,0 ) ) );              // colour
    
    if(PROB_OF_LAUNCH > .92){	// Increase the probability of firing a missile as the game continues
        PROB_OF_LAUNCH -= PROB_OF_LAUNCH/2000;
    }
  }

  // Look for terminating missiles

  for (i=0; i<missilesIn.size(); i++)
    if (missilesIn[i].hasReachedDestination()) {
      // CHANGE THIS: ADD AN EXPLOSION

      explosions.add(Circle(missilesIn[i].position(), 0.2, 0.04, vec3(1, 0, 1)));
      missilesIn.remove(i);
      i--;
    }

  for (i=0; i<missilesOut.size(); i++)
    if (missilesOut[i].hasReachedDestination()) {
      // CHANGE THIS: ADD AN EXPLOSION
      explosions.add(Circle(missilesOut[i].position(), 0.2, 0.04, vec3(1, 1, 0)));
      missilesOut.remove(i);
      i--;
    }

  // Look for terminating explosions

  for (i=0; i<explosions.size(); i++)
    if (!explosions[i].canRemove() && explosions[i].radius() >= explosions[i].maxRadius() ) {
      // CHANGE THIS: CHECK FOR DESTROYED CITY OR SILO
      
      for (int siloInd=0; siloInd<silos.size(); siloInd++) {
        if ((silos[siloInd].position() - explosions[i].getPos()).length() <= explosions[i].radius()) {
          silos.remove(siloInd);
        }
      }

      for (int cityInd=0; cityInd<cities.size(); cityInd++) {
        if (cities[cityInd].isHit(explosions[i].getPos(), explosions[i].radius())) {
          cities.remove(cityInd);
        }
      }

  // Look for incoming missiles that hit an explosion and are
  // destroyed

  // ADD CODE HERE

      for (int missileInd=0; missileInd<missilesIn.size(); missileInd++) {
        if ((missilesIn[missileInd].position() - explosions[i].getPos()).length() <= explosions[i].radius()) {
          missilesIn.remove(missileInd);
        }
      }
      explosions[i].implode();
    }
    else if(explosions[i].canRemove()){
      explosions.remove(i);
      i--;
    }



  // Update all the moving objects

  for (i=0; i<missilesIn.size(); i++)
    missilesIn[i].move( deltaT );

  for (i=0; i<missilesOut.size(); i++)
    missilesOut[i].move( deltaT );

  for (i=0; i<explosions.size(); i++)
    explosions[i].expand( deltaT );
}


// Fire a missile

void State::fireMissile( int siloIndex, float x, float y )

{
  const float speed = 0.2;
    
  if (siloIndex < silos.size() && silos[siloIndex].canShoot()) {

    // CHANGE THIS

    silos[siloIndex].decrMissiles();
    silos[siloIndex].changeBarrel(vec3(x,y,0));

    
    missilesOut.add( Missile( silos[siloIndex].position(),           // source
			      (speed * (vec3(x,y,0) - silos[siloIndex].position())).normalize(), // velocity
			      y,		                     // destination y
			      vec3( 0,1,1 ) ) );                     // colour
  }
}



// Create the initial state of the world


void State::setupWorld()

{
  // Keep track of the time

  currentTime = 0;

  timeOfIncomingFlight = 6;	// 6 seconds for incoming missile to arrive

  // Seed the random number generator

#ifdef _WIN32
  srand( 12345 );
#else
  struct timeb t;
  ftime( &t );
  srand( t.time );
#endif

  // Silos are added in order left, middle, right so that they have
  // indices 0, 1, and 2 (respectively) in the "silos" array.

  silos.add(  Silo( vec3( 0.1, 0, 0 ) ) );
  silos.add(  Silo( vec3( 0.5, 0, 0 ) ) );
  silos.add(  Silo( vec3( 0.9, 0, 0 ) ) );

  // Cities on the left

  cities.add( City( vec3( 0.2, 0, 0 ) ) );
  cities.add( City( vec3( 0.3, 0, 0 ) ) );
  cities.add( City( vec3( 0.4, 0, 0 ) ) );

  // Cities on the right

  cities.add( City( vec3( 0.6, 0, 0 ) ) );
  cities.add( City( vec3( 0.7, 0, 0 ) ) );
  cities.add( City( vec3( 0.8, 0, 0 ) ) );
}
