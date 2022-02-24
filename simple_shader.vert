#version 450

// hardcoded triangle coordinates
vec2 positions[3] = vec2[] (
	vec2(0.0, -0.5),
	vec2(0.5, 0.5),
	vec2(-0.0, 0.5)
);

void main() {
	// gl_Position - output of main function, 4 dimensional vector, center at (0,0)
	// gl_Vertex array position of pixel being calculated
	gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0); 
}