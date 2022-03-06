#version 450

// vertex attribute
layout(location = 0) in vec2 position; // values from vertex buffer
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor; // color output to fragment shader

void main() {
	// gl_Position - output of main function, 4 dimensional vector, center at (0,0)
	// gl_Vertex array position of pixel being calculated
	gl_Position = vec4(position, 0.0, 1.0); 
	fragColor = color;
}