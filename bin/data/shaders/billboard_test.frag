/*
	billboard_test.frag
*/
#version 400

// Uniforms
uniform sampler2D billboard;

// Varying
in vec2 v_TexCoords;

out vec4 out_Color;

void main()
{
	vec4 color = texture(billboard, v_TexCoords);
	
	if(color.a <= 0.5)
	{
		discard;
	}
	
	out_Color = color;

	//out_Color = vec4(1.0);
}