/*
	grass.vert
*/
#version 400

// Attribute
layout(location = 0) in vec3 vertices;
layout(location = 1) in vec2 texCoords;
// 2, 3, 4, 5
layout(location = 2) in mat4 model;

// Uniform
uniform mat4 proj;
uniform mat4 view;

// Varying
out vec2 v_TexCoords;

void main()
{
	gl_Position = proj * view * model * vec4(vertices, 1.0);
	v_TexCoords = texCoords;
}