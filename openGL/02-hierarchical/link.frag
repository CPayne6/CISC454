// fragment shader

#version 300 es

smooth in mediump vec3 colour;
smooth in mediump vec3 normal;

out mediump vec4 outputColour;


void main()

{
  outputColour = vec4( colour, 1.0 );
}
