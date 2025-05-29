#version 330 core
// glVertexAttribPointer mapped positions to third
layout(location = 2) in vec3 in_Position;
// glVertexAttribPointer mapped color  to forth attribute 
layout(location = 3) in vec3 in_Color;

//Matrix Uniforms uploaded with glUniform*
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 pass_Color;

void main() {
	gl_PointSize = 2.0;
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(in_Position, 1.0);
	pass_Color = in_Color;
}