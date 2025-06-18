#version 330 core

in vec3 frg_normal;   // Normal in camera/eye space (already normalized)
in vec3 frg_position; // Position in camera/eye space

// same uniforms as the original shader
uniform vec3 LightPosition; // Light position in camera/eye space
uniform vec3 LightColor;
uniform float LightIntensity;
uniform vec3 PlanetColor;
uniform vec3 EmissiveColor; // sun only


uniform float OutlineThreshold = 0.4; 

out vec4 frag_color;

void main() {
    //Sun is emissive
    if (EmissiveColor != vec3(0.0, 0.0, 0.0)) {
        frag_color = vec4(EmissiveColor, 1.0);
        return;
    }

    // Define direction vectors
    vec3 normal = frg_normal; // Already normalized from vertex shader
    vec3 view_dir = normalize(-frg_position); // Direction from fragment to camera (origin in view space)

    //  Outline effect 
    if (dot(normal, view_dir) < OutlineThreshold) {
        frag_color = vec4(PlanetColor, 1.0);
        return; // Exit after drawing the outline pixel
    }

    vec3 ambient = 0.1 * PlanetColor; // ambient strength

    // Light direction (from fragment to light source)
    vec3 light_dir = normalize(LightPosition - frg_position);


    // Diffuse component (how much light reflects based on angle to light)
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * LightColor * PlanetColor * LightIntensity;

    // Specular component (highlights, reflections)
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0); // 32.0 is shininess exponent
    vec3 specular = spec * LightColor * LightIntensity * 0.5; // specular color strength

    // Final lit color
    frag_color = vec4(ambient + diffuse + specular, 1.0);
}