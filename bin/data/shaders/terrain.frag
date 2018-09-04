/**
	terrain.frag
	
	This is the shader that controls the fragment portion of 
	the video card.
*/
#version 400

// Uniform
uniform float tiling;
uniform sampler2D blendMap;
uniform sampler2D channelBlack;
uniform sampler2D channelRed;
uniform sampler2D channelGreen;
uniform sampler2D channelBlue;

// Varying
in vec2 v_TexCoords;
out vec4 out_Color;

void main()
{
	//out_Color = texture(blendMap, v_TexCoords);
	
	vec3 bm = texture(blendMap, v_TexCoords).rgb;
	
	vec3 cb = texture(channelBlack, v_TexCoords * tiling).rgb * ( 1.0 - (bm.r + bm.g + bm.b));
	vec3 cr = texture(channelRed, v_TexCoords * tiling).rgb;
	vec3 cg = texture(channelGreen, v_TexCoords * tiling).rgb;
	vec3 cbl = texture(channelBlue, v_TexCoords * tiling).rgb;
	
	// Mixing...
	vec3 color = cr * bm.r + cg * bm.g + cbl * bm.b + cb;
	
	out_Color = vec4(color, 1.0);
}