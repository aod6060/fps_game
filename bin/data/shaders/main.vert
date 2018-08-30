/**
	main.vert
	
	This is the vertex shader for this simple litter game...
*/

#version 400


// Attributes
layout(location = 0) in vec3 vertices;
layout(location = 1) in vec4 vertexColors;

// Uniform
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

// Varying
out vec4 out_VertexColors;

void main()
{
	gl_Position = proj * view * model * vec4(vertices, 1.0);
	out_VertexColors = vertexColors;
}