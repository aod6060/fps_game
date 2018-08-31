/**
	main.vert
	
	This is the vertex shader for this simple litter game...
*/

#version 400

// Uniform
uniform sampler2D tex0;

// Varying
in vec4 v_VertexColors;
in vec2 v_TexCoords;

out vec4 out_Color;

void main()
{
	//out_Color = out_VertexColors;
	out_Color = texture(tex0, v_TexCoords) * v_VertexColors;
}