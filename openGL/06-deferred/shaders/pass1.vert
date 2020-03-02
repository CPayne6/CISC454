// Pass 1 vertex shader
//
// Stores colour, normal, depth

#version 300 es

uniform mat4 M;
uniform mat4 MV;
uniform mat4 MVP;

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec3 vertNormal;

out mediump vec3 colour;
out mediump vec3 normal;
out mediump float depth;

void main()

{
  // calc vertex position in CCS (always required)

  mediump vec4 vccs = MVP * vec4( vertPosition, 1.0 );
  gl_Position = vccs;

  // Provide a colour 

  colour = vec3(0.66, 0.88, 0.33);

  // calculate normal in VCS

  normal = vec4( vertNormal, 0.0 ).xyz;
  //normal = (MV * vec4( vertNormal, 0.0 )).xyz;           // <-------------- !!!

  // Calculate the depth in [0,1].

  depth = (vccs.z/vccs.w+1.0)/2.0;
}
