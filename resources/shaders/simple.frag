#version 330 core

in vec3 frg_normal;    // Normal in camera/eye space
in vec3 frg_position;  // Position in camera/eye space

uniform vec3 LightPosition; // Light position in camera/eye space
uniform vec3 LightColor;
uniform float LightIntensity;
uniform vec3  PlanetColor;  // Base color for lit objects
uniform vec3  EmissiveColor; // <--- NEW/REINTRODUCED UNIFORM for glowing objects

out vec4 frag_color;

void main() {
    // If EmissiveColor is non-zero, this fragment belongs to an emissive object (like the sun)
    if (EmissiveColor != vec3(0.0, 0.0, 0.0)) {
        frag_color = vec4(EmissiveColor, 1.0); // Directly output the emissive color
    } else {
        // Standard lighting calculations for planets

        // Ambient light (a small base light that affects everything)
        vec3 ambient = 0.1 * PlanetColor; // You can adjust the ambient strength

        // Light direction (from fragment to light source)
        vec3 light_dir = normalize(LightPosition - frg_position);

        // Normal should already be normalized from vertex shader
        vec3 normal = normalize(frg_normal);

        // Diffuse component (how much light reflects based on angle to light)
        float diff = max(dot(normal, light_dir), 0.0);
        vec3 diffuse = diff * LightColor * PlanetColor * LightIntensity;

        // Specular component (highlights, reflections)
        vec3 view_dir = normalize(-frg_position); // Camera is at origin in camera space
        vec3 reflect_dir = reflect(-light_dir, normal);
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0); // 32.0 is shininess exponent
        vec3 specular = spec * LightColor * LightIntensity * 0.5; // Example specular color strength

        // Final lit color
        frag_color = vec4(ambient + diffuse + specular, 1.0);
    }
}