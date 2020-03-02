/* rectangle.h
 *
 * The rectangle lies in the y-z plane: [-0.5,0.5] in y and [0,1] in z.
 * The 'transform' is applied to initially pose the rectangle.
 */


#ifndef RECTANGLE_H
#define RECTANGLE_H


#include "geometry.h"


class Rectangle: public Geometry {

 public:

  Rectangle( mat4 transform ) {
    create( transform );
    primitiveType = GL_LINE_LOOP;
  }

  void create( mat4 transform );
};

#endif
