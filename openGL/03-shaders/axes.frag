// axes.frag

#version 300 es

smooth in mediump vec3 colour;
out mediump vec4 outputColour;

void main() {
  outputColour = vec4( colour, 1.0 );
}
