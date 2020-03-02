/* cylinder.h
 *
 * The cylinder is aligned with the z axis, has radius 1, and goes
 * from z=-0.5 to z=0.5.  The 'transform' is applied to initially pose the
 * cylinder.
 */


#ifndef CYLINDER_H
#define CYLINDER_H


#include "geometry.h"


const int numSlices = 128; // number of slices around the cylinder in [0,2pi]


class Cylinder: public Geometry {

 public:

  Cylinder( mat4 transform ) {
    create( transform );
  }

  void create( mat4 transform );
};

#endif
