/*
	blur.frag
*/

#version 400

// Uniform
uniform sampler2D inputTex0;
uniform float size;

// Vayring
in vec2 v_TexCoords;
out vec4 out_Color;


vec3 hblur(sampler2D samp, vec2 tc, float blurSize)
{
	vec3 sum = vec3(0.0);
	float iBlurSize = 1.0 / blurSize;
	
	sum += texture(samp, vec2(tc.x - 4.0 * iBlurSize, tc.y)).xyz * 0.125;
	sum += texture(samp, vec2(tc.x - 3.0 * iBlurSize, tc.y)).xyz * 0.125;
	sum += texture(samp, vec2(tc.x - 2.0 * iBlurSize, tc.y)).xyz * 0.125;
	sum += texture(samp, vec2(tc.x - 1.0 * iBlurSize, tc.y)).xyz * 0.125;
	sum += texture(samp, vec2(tc.x + 4.0 * iBlurSize, tc.y)).xyz * 0.125;
	sum += texture(samp, vec2(tc.x + 3.0 * iBlurSize, tc.y)).xyz * 0.125;
	sum += texture(samp, vec2(tc.x + 2.0 * iBlurSize, tc.y)).xyz * 0.125;
	sum += texture(samp, vec2(tc.x + 1.0 * iBlurSize, tc.y)).xyz * 0.125;
	
	return sum;
}

vec3 vblur(sampler2D samp, vec2 tc, float blurSize)
{
	vec3 sum = vec3(0.0);
	float iBlurSize = 1.0 / blurSize;
	
	sum += texture(samp, vec2(tc.x, tc.y - 4.0 * iBlurSize)).xyz * 0.125;
	sum += texture(samp, vec2(tc.x, tc.y - 3.0 * iBlurSize)).xyz * 0.125;
	sum += texture(samp, vec2(tc.x, tc.y - 2.0 * iBlurSize)).xyz * 0.125;
	sum += texture(samp, vec2(tc.x, tc.y - 1.0 * iBlurSize)).xyz * 0.125;
	
	sum += texture(samp, vec2(tc.x, tc.y + 4.0 * iBlurSize)).xyz * 0.125;
	sum += texture(samp, vec2(tc.x, tc.y + 3.0 * iBlurSize)).xyz * 0.125;
	sum += texture(samp, vec2(tc.x, tc.y + 2.0 * iBlurSize)).xyz * 0.125;
	sum += texture(samp, vec2(tc.x, tc.y + 1.0 * iBlurSize)).xyz * 0.125;
	
	return sum;
}

void main()
{
	vec3 hb = hblur(inputTex0, v_TexCoords, size);
	vec3 vb = vblur(inputTex0, v_TexCoords, size);
	vec3 i = texture(inputTex0, v_TexCoords).xyz;
	
	out_Color = vec4(hb * 0.334 + vb * 0.334 + i * i, 1.0);
	
	//out_Color = texture(inputTex0, v_TexCoords);
}
