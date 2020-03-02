/* link.h
 *
 * One link on a robot arm.  This is lined up along the y axis with
 * the origin 0.25 units below the arm.
 */


#ifndef LINK_H
#define LINK_H


#include "linalg.h"
#include "object.h"
#include "cylinder.h"
#include "rectangle.h"


const float diameter     =  0.5;
const float originOffset =  0.25;
const float length       =  2.0;


class Link : public Object {

 public:

  Link() {

#if 0

    mat4 config =
      translate( 0, 0.5*length + originOffset, 0 ) *
      scale( diameter, length, 1 );

    geometry = new Rectangle( config ); // default [-0.5,0.5] x [-0.5,0.5] x [0,0]

#else

    mat4 config =
      translate( 0, 0.5*length + originOffset, 0 ) *
      scale( diameter, length, diameter ) *
      rotate( -90*M_PI/180.0, vec3(1,0,0) );

    geometry = new Cylinder( config ); // default diameter 1, length 1, centred at origin, axis along z

#endif
  }
};


#endif
