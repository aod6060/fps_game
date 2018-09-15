/*
	test_billboard.frag
*/
#version 400

// Uniform
uniform sampler2D tex0;
uniform vec3 colorize;

// Varying
in vec2 v_TexCoords;
out vec4 out_Color;

void main()
{
	vec4 color = texture(tex0, v_TexCoords);
	
	/*
	if(color.a < 0.5)
	{
		discard;
	}
	*/
	
	out_Color = vec4(color.rgb * colorize.rgb, color.a);
}