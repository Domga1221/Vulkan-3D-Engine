#version 450

// vertex attribute
layout(location = 0) in vec3 position; // values from vertex buffer
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push {
	mat4 transform;
	vec3 color;
} push;

void main() {
	// gl_Position - output of main function, 4 dimensional vector, center at (0,0)
	// gl_Vertex array position of pixel being calculated
	gl_Position = push.transform * vec4(position, 1.0); // shift vertex by the same amount, moving the entire model
	fragColor = color;
}