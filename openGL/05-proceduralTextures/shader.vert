// shader.vert
//
// OLD VERSION OF GLSL ... not 3.00+

const float major = 5.0;     // major radius
const float minor = 2.0;     // minor radius


void main()

{

  // map spatial coordinates from
  //
  //     [-(major+minor),+(major+minor)]
  //   x [-(major+minor),+(major+minor)]
  //   x [-minor,+minor]
  //
  // to texture coordinates in
  //
  //   [0,1] x [0,1] x [0,1]

  gl_TexCoord[0] = vec4( (gl_Vertex.x + (major+minor))/(2.0*(major+minor)),
                         (gl_Vertex.y + (major+minor))/(2.0*(major+minor)),
			 (gl_Vertex.z + minor)/(2.0*minor),
                         0.0 );

  gl_Position = ftransform();
  gl_FrontColor = gl_Color;
}
