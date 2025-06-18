#version 330 core 

layout(location = 0) in vec3 in_position; // Vertex position 
layout(location = 1) in vec3 in_normal;   // Vertex normal 
uniform mat4 ModelMatrix;      // Model to world transformation.
uniform mat4 ViewMatrix;       // World to camera transformation.
uniform mat4 ProjectionMatrix; // Camera to clip space transformation.
uniform mat4 NormalMatrix;     // Normal transformation to camera space.

out vec3 frg_normal;   // Interpolated normal for fragment shader (camera space).
out vec3 frg_position; // Interpolated position for fragment shader (camera space).

void main() {
    //outputs for fragment shader
    frg_position = vec3(ViewMatrix * ModelMatrix * vec4(in_position, 1.0));
    frg_normal = normalize(vec3(NormalMatrix * vec4(in_normal, 0.0)));
    
    //final vertex position 
    gl_Position = ProjectionMatrix * vec4(frg_position, 1.0);
}