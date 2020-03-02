// Pass 2 vertex shader

#version 300 es

layout (location = 0) in vec2 vertPosition;
layout (location = 1) in vec2 vertTexCoords;

out mediump vec2 texCoords;

void main()

{
  gl_Position = vec4( vertPosition, 0.5, 1.0 );
  texCoords = vertTexCoords;
}
