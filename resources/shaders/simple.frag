#version 330 core

// Inputs interpolated from vertex shader
in vec3 vNormal;    // Normal vector at the fragment in world space
in vec3 vPosition;  // Position of the fragment in world space

// Uniforms passed from CPU
uniform vec3 LightPosition;  // Position of the light source in world space
uniform vec3 LightColor;     // Color/intensity of the light source

// Output fragment color
out vec4 FragColor;

void main() {
    // Normalize interpolated normal vector
    vec3 normal = normalize(vNormal);

    // Calculate vector from fragment to light source
    vec3 lightDir = normalize(LightPosition - vPosition);

    // Compute diffuse lighting factor using Lambert's cosine law (dot product of normal and light direction)
    float diff = max(dot(normal, lightDir), 0.0);

    // Calculate diffuse light contribution (scaled by light color)
    vec3 diffuse = diff * LightColor;

    // Ambient light contribution (constant low intensity to simulate indirect light)
    vec3 ambient = 0.1 * LightColor;

    // Sum ambient and diffuse lighting
    vec3 result = ambient + diffuse;

    // Output final color with full opacity
    FragColor = vec4(result, 1.0);
}
