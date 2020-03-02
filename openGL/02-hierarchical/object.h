// object.h


#ifndef OBJECT_H
#define OBJECT_H


#include <iostream>
#include <string>
#include <vector>
#include "linalg.h"
#include "geometry.h"

using namespace std;


class Object {

  // object pose.  To convert to parent's coordinate system, transformation is M = TRS:

  vec3       position;		// use for T
  quaternion orientation;	// use for R
  float      scaleFactor;	// use for S

  // descendants

  vector<Object*> children;

 public:

  Geometry *geometry;  // Stuff to draw (can be NULL)

  string         name;
  vector<string> childNames;	// (used only for reading)
  bool           selected;

  Object() {
    reset();
    geometry = NULL;
  }

  Object( Geometry *g ) {
    reset();
    geometry = g;
  }

  Object( string n, Geometry *g ) {
    reset();
    name = n;
    geometry = g;
  }

  void reset() {
    selected = false;
    scaleFactor = 1;
    position = vec3(0,0,0);
    orientation = quaternion( 0, vec3(0,0,1) );
    selected = false;
  }

  void draw( mat4 &parentM, mat4 &parentMV, mat4 &parentMVP, vec3 &lightDir, unsigned int programID );

  void addChild( Object *child ) {
    children.push_back( child );
  }

  void rotateObj( float theta, vec3 axis ) {
    orientation = orientation * quaternion( theta, axis );
  }

  void translateObj( vec3 v ) {
    position = position + v;
  }

  void scaleObj( float s ) {
    scaleFactor = scaleFactor * s;
  }

  mat4 objToParentTransform() {

    mat4 M
      = translate( position )
      * orientation.toMatrix()
      * scale( scaleFactor, scaleFactor, scaleFactor );

    return M;
  }

  friend ostream& operator << ( ostream& stream, Object const & obj );
  friend istream& operator >> ( istream& stream, Object & obj );

};

ostream& operator << ( ostream& stream, Object const & obj );
istream& operator >> ( istream& stream, Object & obj );

#endif
