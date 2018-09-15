/*
	particle.frag
*/
#version 400

// Uniform
uniform sampler2D tex0;
uniform vec3 color;

// Varying
in vec2 v_TexCoords;

out vec4 out_Color;

void main()
{
	vec4 c = texture(tex0, v_TexCoords);
	if(c.a >= 0.5)
	{
		out_Color = vec4(c.rgb * color, 1.0);
	}
	else
	{
		out_Color = vec4(c.rgb * color, c.a);
	}
}