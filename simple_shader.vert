#version 450

// vertex attribute
layout(location = 0) in vec2 position; // values from vertex buffer
layout(location = 1) in vec3 color;

layout(push_constant) uniform Push {
	vec2 offset; // order must match struct 
	vec3 color;
} push;

void main() {
	// gl_Position - output of main function, 4 dimensional vector, center at (0,0)
	// gl_Vertex array position of pixel being calculated
	gl_Position = vec4(position + push.offset, 0.0, 1.0);  // shift vertex by the same amount, moving the entire model

}