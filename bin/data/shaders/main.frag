/**
	main.vert
	
	This is the vertex shader for this simple litter game...
*/

#version 400


// Varying
in vec4 out_VertexColors;
out vec4 out_Color;

void main()
{
	out_Color = out_VertexColors;
}