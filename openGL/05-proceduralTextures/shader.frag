// shader.frag
//
// Perlin noise wood grain
//
// OLD VERSION OF GLSL ... not 3.00+


uniform sampler2D noise;
uniform int renderMode;
uniform float perlinLevels;
uniform float captureRadius;

const vec3 brown1 = vec3( 205.0/255.0, 143.0/255.0, 73.0/255.0 );
const vec3 brown2 = vec3( 119.0/255.0,  69.0/255.0, 19.0/255.0 );


// Return random Perlin in [0,1] with A=2, B=2

float perlinNoise( vec2 pos )

{
  float sum = 0.0;
  float maxSum = 0.0;
  float scale = 1.0;

  for (float i=0.0; i<perlinLevels; i++) {

    sum = sum + texture2D( noise, scale*pos ).x / scale;

    maxSum = maxSum + 1.0/scale;

    scale = 2.0 * scale;
  }

  return sum / maxSum; // normalize to [0,1]
}



void main()

{
  vec2 texCoords = gl_TexCoord[0].st;

  float x = texCoords.x;
  float offset;

  vec3 colour = vec3(0,0,0);

  switch (renderMode) {
  case 0: // TEX_COORDS
    colour = gl_TexCoord[0].stp;
    break;
  case 1: // NOISE TEXTURE
    float p = perlinNoise(texCoords);
    colour = vec3( p, p, p );
    break;
  case 2: // SIN
    colour = mix( brown1, brown2, sin( 6.28*x )*0.5+0.5 );
    break;
  case 3: // SIN100
    colour = mix( brown1, brown2, sin( 100.0*x )*0.5+0.5 );
    break;
  case 4: // PERLIN
    offset = captureRadius * ((perlinNoise(texCoords) - 0.5) * 2.0);
    colour = mix( brown1, brown2, sin( 100.0*(x + offset) )*0.5+0.5 );
    break;
  case 5: // PERLIN_EXP
    offset = captureRadius * ((perlinNoise(texCoords) - 0.5) * 2.0);
    colour = mix( brown1, brown2, pow( sin( 100.0*(x + offset) )*0.5+0.5, 3.0 ) );
    break;
  }

  gl_FragColor = vec4( colour, 1.0 );
}


