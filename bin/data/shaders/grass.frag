/*
	grass.frag
*/
#version 400

// Uniform
uniform sampler2D grass;

// Varying
in vec2 v_TexCoords;
out vec4 out_Color;

void main()
{
	vec4 color = texture(grass, v_TexCoords);
	
	if(color.a <= 0.5)
	{
		discard;
	}
	
	
	out_Color = color;
}
