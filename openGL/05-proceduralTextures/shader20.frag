// shader.frag
//
// Perline noise wood grain


uniform sampler2D noise;
uniform int renderMode;

const vec3 brown1 = vec3( 205.0/255.0, 133.0/255.0, 63.0/255.0 );
const vec3 brown2 = vec3( 139.0/255.0,  69.0/255.0, 19.0/255.0 );


float perlinNoise( vec2 pos )

{
  float sum = 0.0;
  float scale = 0.5;   // normally = 1.0, but the base noise is too high-frequency already

  for (float i=0.0; i<4.0; i++) {
    sum = sum + texture2D( noise, scale*pos ).x / scale;
    scale = 2.0 * scale;
  }

  return clamp( 0.03 * sum, 0.0, 1.0 );
}



void main()

{
  vec2 texCoords = gl_TexCoord[0].st;

  float x = texCoords.x;

  vec3 colour = vec3(0,0,0);

  switch (renderMode) {
  case 0: // TEX_COORDS
    colour = gl_TexCoord[0].stp;
    break;
  case 1: // SIN
    colour = mix( brown1, brown2, sin( x ) );
    break;
  case 2: // SIN100
    colour = mix( brown1, brown2, sin( 100.0*x ) );
    break;
  case 3: // PERLIN
    colour = mix( brown1, brown2, sin( 100.0*(x + perlinNoise(texCoords)) ) );
    break;
  case 4: // PERLIN_EXP
    colour = mix( brown1, brown2, pow( sin( 100.0*(x + perlinNoise(texCoords)) ), 3.0 ) );
    break;
  }

  gl_FragColor = vec4( colour, 1.0 );
}


