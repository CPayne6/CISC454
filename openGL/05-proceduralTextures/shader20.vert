// shader.vert

const float r12 = 7.0;    // max distance from origin in x and y
const float r2 = 2.0;     // max distance from origin in z


void main()

{

  // map spatial coordinates (x,y,z) to texture coordinates in [0,1]x[0,1]x[0,1]

  gl_TexCoord[0] = vec4( (gl_Vertex.x + r12)/(2.0*r12),
                         (gl_Vertex.y + r12)/(2.0*r12),
			 (gl_Vertex.z + r2)/(2.0*r2),
                         0.0 );

  gl_Position = ftransform();
  gl_FrontColor = gl_Color;
}
