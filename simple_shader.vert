#version 450

// vertex attribute
layout(location = 0) in vec2 position; // values from vertex buffer

void main() {
	// gl_Position - output of main function, 4 dimensional vector, center at (0,0)
	// gl_Vertex array position of pixel being calculated
	gl_Position = vec4(position, 0.0, 1.0); 
}