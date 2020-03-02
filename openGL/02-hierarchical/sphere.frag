// fragment shader

#version 300 es

uniform vec3 lightDir; // direction *toward* light in VCS

flat in mediump vec3 colour;
smooth in mediump vec3 normal;

out vec4 mediump outputColour;


void main()

{
  // attenuate colour by cosine of angle between fragment normal and
  // light direction.  But ... if dot product is negative, light is
  // behind fragment and fragment is black (on can appear grey if some
  // "ambient" light is added).

  float NdotL = dot( normalize(normal), lightDir );
  if (NdotL < 0.0)
    NdotL = 0.0;

  vec3 diffuseColour = NdotL * colour;

  float ambient = 0.2;
  outputColour = vec4( (1.0-ambient) * diffuseColour + ambient * vec3(1.0,1.0,1.0), 1.0 );
}
