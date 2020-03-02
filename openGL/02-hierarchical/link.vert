// vertex shader

#version 300 es

uniform vec3 vertColour;
uniform mat4 MVP;
uniform mat4 MV;

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec3 vertNormal;

out mediump vec3 colour;
out mediump vec3 normal;


void main()

{
  gl_Position = MVP * vec4( vertPosition, 1.0 );
  colour = vertColour;
  normal = (MV * vec4( vertNormal, 1.0 )).xyz;
}
