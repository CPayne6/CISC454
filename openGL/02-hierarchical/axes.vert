// axes.vert

#version 300 es

uniform mat4 MVP;

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec3 vertColour;

smooth out mediump vec3 colour;

void main() {
  gl_Position = MVP * vec4( vertPosition, 1.0f );
  colour = vertColour;
}
