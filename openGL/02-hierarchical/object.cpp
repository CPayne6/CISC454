// object.cpp


#include "object.h"


// Draw the object and its children

void Object::draw( mat4 &parentM, mat4 &parentMV, mat4 &parentMVP, vec3 &lightDir, unsigned int programID )

{
  // Update transform to parent

  mat4 M   = parentM   * objToParentTransform();
  mat4 MV  = parentMV  * objToParentTransform();
  mat4 MVP = parentMVP * objToParentTransform();

  // Draw geometry

  if (geometry != NULL) {

    // highlight selected object

    vec3 vertColour = (selected ? vec3( 0.9, 0.4, 0.2 ) : vec3( 1.0, 1.0, 0.0 ));
    glUniform3fv( glGetUniformLocation( programID, "vertColour"), 1, &vertColour[0] );

    // Draw it

    geometry->draw( M, MV, MVP, lightDir, programID );
  }

  // Recursively draw children

  for (unsigned int i=0; i<children.size(); i++)
    children[i]->draw( M, MV, MVP, lightDir, programID );
}



// I/O operators

std::ostream& operator << ( std::ostream& stream, Object const& obj )

{
  stream << obj.name << std::endl
	 << "  " << obj.scaleFactor << std::endl
	 << "  " << obj.position    << std::endl
	 << "  " << obj.orientation << std::endl
	 << "  " << obj.children.size();

  for (unsigned int i=0; i<obj.children.size(); i++)
    stream << " " << obj.children[i]->name;
  
  stream << std::endl;
  
  return stream;
}

std::istream& operator >> ( std::istream& stream, Object & obj )

{
  unsigned int numChildren;

  if (stream) 
    stream >> obj.name
	   >> obj.scaleFactor
	   >> obj.position
	   >> obj.orientation
	   >> numChildren;

  if (stream)
    for (unsigned int i=0; i<numChildren; i++) {
      string childName;
      stream >> childName;
      obj.childNames.push_back( childName );
    }

  return stream;
}

