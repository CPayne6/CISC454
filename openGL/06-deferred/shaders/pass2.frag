// Pass 2 fragment shader

#version 300 es

uniform mediump vec3 lightDir;
uniform sampler2D colourSampler;
uniform sampler2D normalSampler;
uniform sampler2D depthSampler;

in mediump vec2 texCoords;
out mediump vec4 outputColour;


void main()

{
  mediump vec3 Iin = vec3( 1.0, 1.0, 1.0 );
  mediump vec3 Ia  = vec3( 0.0, 0.0, 0.0 );
  mediump vec3 ks  = vec3( 0.8, 0.8, 0.8 );
  mediump float shininess = 400.0;

  mediump vec3  kd    = texture( colourSampler, texCoords ).rgb;
  mediump vec3  N     = normalize( texture( normalSampler , texCoords  ).rgb );
  mediump float depth = texture( depthSampler, texCoords ).r;

  if (depth == 1.0)

    // Background

    outputColour = vec4( 1.0, 0.7, 0.3, 1.0 );

  else {

    // Phong lighting

    mediump float NdotL = dot( N, lightDir );
    mediump vec3 Iout = Ia;
    if (NdotL > 0.0) {
      Iout += NdotL * vec3( kd.r * Iin.r, kd.g * Iin.g, kd.b * Iin.b ); 
      mediump vec3 R = (2.0 * NdotL) * N - lightDir;
      mediump vec3 V = vec3(0.0,0.0,1.0);	// direction toward eye in the VCS
      mediump float RdotV = dot( R, V );
      Iout += pow( RdotV, shininess ) * vec3( ks.r * Iin.r, ks.g * Iin.g, ks.b * Iin.b );
      outputColour = vec4( Iout, 1.0 );
    } else
      outputColour = vec4(0.3, 0.3, 0.3, 1);
  }
}
