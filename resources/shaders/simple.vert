#version 330 core

// Input vertex attributes: position and normal
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

// Uniform matrices for transformations
uniform mat4 ModelMatrix;       // Model transformation matrix (model space -> world space)
uniform mat4 ViewMatrix;        // View matrix (world space -> camera/view space)
uniform mat4 ProjectionMatrix;  // Projection matrix (camera space -> clip space)
uniform mat4 NormalMatrix;      // Matrix to correctly transform normals (usually inverse transpose of ModelMatrix)

// Output variables to fragment shader
out vec3 vNormal;    // Transformed normal vector in world space
out vec3 vPosition;  // Position of the vertex in world space

void main() {
    // Transform vertex position from model space to world space
    vec4 worldPosition = ModelMatrix * vec4(position, 1.0);

    // Pass world space position to fragment shader
    vPosition = vec3(worldPosition);

    // Transform normal vector using NormalMatrix and normalize it
    vNormal = normalize(mat3(NormalMatrix) * normal);

    // Calculate final clip space position by applying projection and view transformations
    gl_Position = ProjectionMatrix * ViewMatrix * worldPosition;
}
