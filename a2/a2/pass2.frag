// Pass 2 fragment shader
//
// Apply diffuse lighting to fragment.  Later do Phong lighting.
//
// Determine whether fragment is in shadow.  If so, reduce intensity to 50%.

#version 300 es
precision mediump float;


uniform vec3      lightDir;	    // direction to light in WCS
uniform vec3      eyePosition;	    // position of eye in WCS
uniform mat4      WCS_to_lightCCS;  // transform from WCS to light's CCS
uniform sampler2D shadowBuffer;     // texture [0,1]x[0,1] of depth from light.  Values in [0,1].
uniform sampler2D objTexture;       // object's texture (might not be provided)
uniform bool      texturing;        // =1 if object's texture is provided

in vec3 colour;        // fragment colour
in vec3 normal;        // fragment normal in WCS
in vec3 wcsPosition;   // fragemnt position in WCS
in vec2 texCoords;     // fragment texture coordinates (if provided) 

out vec4 fragColour;   // fragment's final colour

void main()

{
  // Set the colour of the fragment (used to simplify texture proccess)
  vec3 color = (texturing)?(texture(objTexture, texCoords).rgb):(colour);
  // Calculate the position of this fragment in the light's CCS.
  
  vec4 ccsLightPos = WCS_to_lightCCS * vec4(wcsPosition, 1.0f); // CHANGE THIS

  // Calculate the depth of this fragment in the light's CCS in the range [0,1]
  //ccsLightDepth = 0.5 * ((ccsLightPos.z / ccsLightPos.w) + 1.0);

  float fragDepth = 0.5 * ((ccsLightPos.z / ccsLightPos.w) + 1.0); // CHANGE THIS

  // Determine the (x,y) coordinates of this fragment in the light's
  // CCS in the range [0,1]x[0,1].

  vec2 shadowTexCoords = vec2(WCS_to_lightCCS * vec4(texCoords,1.0f,1.0f)); // CHANGE THIS

  // Look up the depth from the light in the shadowBuffer texture.

  float shadowDepth = 0.25; // CHANGE THIS

  // Determine whether the fragment is in shadow.
  //
  // If results look bad, add a bit to the shadow texture depth to
  // prevent z-fighting.

  vec3 projCoords = ccsLightPos.xyz;

  projCoords = projCoords * 0.5 + 0.5;
  float closestDepth = texture(shadowBuffer, projCoords.xy).r; 
  float currDepth = projCoords.z;
  float bias = 0.005;
  float shadow = currDepth - bias > closestDepth  ? 1.0 : 0.0;
  if(projCoords.z > 1.0)
    shadow = 0.0;

  // Compute illumination.  Initially just do diffuse "N dot L".  Later do Phong.
  vec3 lightColor = vec3(0.5);
  // ambient
  vec3 ambient = 0.3 * color;
  // diffuse
  float diff =  max(dot(lightDir, normal), 0.0); //set to 0 if negative
  vec3 diffuse = diff * lightColor;

  // specular
  vec3 viewDir = vec3(WCS_to_lightCCS * vec4(normalize(eyePosition - wcsPosition),1.0f));
  vec3 halfDir = vec3(WCS_to_lightCCS * vec4(normalize(lightDir + viewDir),1.0f));
  vec3 spec = pow(max(dot(normal, halfDir), 0.0), 64.0) * lightColor;

  // Choose the colour either from the object's texture (if
  // 'texturing' == 1) or from the input colour.
  
  // This is handled at the beginning of the function***
  
  // YOUR CODE HERE
  // Output the fragment colour, modified by the illumination model
  // and shadowing.
  vec3 lighting;
  if(texturing)
    lighting = (ambient + (1.0 - shadow) * (diffuse + spec)) * color;
  else
    lighting = (ambient + (1.0 - shadow) * (diffuse * 0.07 + spec)) * color; // reduce the effect of diffusive lighting on colours

  fragColour = vec4( lighting, 1.0 );	// CHANGE THIS
}
