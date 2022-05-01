#version 330 core

layout(location = 0) in vec3 Position;
/*LABB 3*/
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoord;

out vec3 interpolatedNormal;
out vec2 st;
out vec3 lightDirection;

uniform float time;
uniform mat4 R, MV, P;


void main() {
    //gl_Position = T * vec4(Position, 1.0);
	//gl_Position = R * vec4(Position, abs(sin(time)));
	//gl_Position = MV * vec4(Position, 1.0);
	
	
	vec3 transformedNormal = mat3(MV) * Normal;
	interpolatedNormal = normalize(transformedNormal);
	lightDirection = mat3(R) * vec3(1.0, 0.8, 1.0);
	gl_Position = P * MV * vec4(Position, 1.0); // Special, required output
	
	st = TexCoord; // Will also be interpolated across the triangle
}
