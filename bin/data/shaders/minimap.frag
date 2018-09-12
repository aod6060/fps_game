/*
	minimap.frag
*/
#version 400
// Uniforms
uniform sampler2D tex0;

// Varying
in vec2 v_TexCoords;
out vec4 out_Color;

void main()
{
	out_Color = texture(tex0, v_TexCoords);
	//out_Color = vec4(1.0);
}