#version 330 core 

layout(location = 0) in vec3 in_position; // Vertex position (model space).
layout(location = 1) in vec3 in_normal; 	// Vertex normal (model space).
layout(location = 2) in vec2 in_texcoord;   //  Vertex texture coordinates 

uniform mat4 ModelMatrix; 		// Model to world transformation.
uniform mat4 ViewMatrix; 		// World to camera transformation.
uniform mat4 ProjectionMatrix; 	// Camera to clip space transformation.
uniform mat4 NormalMatrix; 		// Normal transformation to camera space.

out vec3 frg_normal; 	// Interpolated normal for fragment shader (camera space).
out vec3 frg_position; 	// Interpolated position for fragment shader (camera space).
out vec2 frg_texcoord;  // Interpolated texture coordinates for fragment shader.

void main() {
    frg_position = vec3(ViewMatrix * ModelMatrix * vec4(in_position, 1.0)); // Transform position to camera space.
    frg_normal = normalize(vec3(NormalMatrix * vec4(in_normal, 0.0))); 	// Transform and normalize normal to camera space.
    frg_texcoord = in_texcoord; //  Pass texture coordinates directly
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(in_position, 1.0); // Final vertex position in clip space.
}