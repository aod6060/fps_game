/**
	main.vert
	
	This is the vertex shader for this simple litter game...
*/

#version 400

// Uniform
uniform sampler2D tex0;
uniform samplerCube cube0;
uniform vec3 color;

// Varying
in vec2 v_TexCoords;
in vec3 v_Normals;
in vec3 v_TexCoords2;

out vec4 out_Color;

void main()
{
	//out_Color = out_VertexColors;
	//out_Color = texture(tex0, v_TexCoords);
	//out_Color = vec4(1.0);
	out_Color = vec4(texture(cube0, v_TexCoords2).rgb * color, 1.0);
}