#version 330 core

in vec3 frg_normal;     // Normal in camera/eye space
in vec3 frg_position;   // Position in camera/eye space
in vec2 frg_texcoord;   // Interpolated texture coordinates

uniform vec3 LightPosition; // Light position in camera/eye space
uniform vec3 LightColor;
uniform float LightIntensity;
uniform vec3 PlanetColor;   // Base color for lit objects (will be multiplied with texture)
uniform vec3 EmissiveColor; //for glowing objects

uniform sampler2D PlanetTexture; // Texture sampler uniform
uniform bool HasTexture;         // Flag to indicate if texture exists

out vec4 frag_color;

void main() {
    //Sun is emissive
    if (EmissiveColor != vec3(0.0, 0.0, 0.0) && HasTexture) {
        frag_color = vec4(vec3(texture(PlanetTexture, frg_texcoord)), 1.0); // Directly output the emissive color
        return;
    } else if (EmissiveColor != vec3(0.0, 0.0, 0.0) && !HasTexture) {
        frag_color = vec4(EmissiveColor, 1.0); // Directly output the emissive color
        return;
    }

    vec3 object_color = PlanetColor; // Declare object_color 
    if (HasTexture) {
        object_color = vec3(texture(PlanetTexture, frg_texcoord));
    }

    // Define direction vectors
    vec3 normal = normalize(frg_normal); // Ensure normal is normalized
    vec3 view_dir = normalize(-frg_position); // Direction from fragment to camera (origin in view space)

    // Outline effect (more of a rim/back-facing color in this context)
    // If the angle between the normal and view_dir is large (surface facing away)
    if (dot(normal, view_dir) < 0.6) {
        frag_color = vec4(PlanetColor, 1.0); // Use a distinct outline color
        return; // Exit after drawing the outline pixel
    }

    vec3 ambient = 0.1 * object_color; // ambient strength

    // Light direction (from fragment to light source)
    vec3 light_dir = normalize(LightPosition - frg_position);

    // Diffuse component (how much light reflects based on angle to light)
    float diff = max(dot(normal, light_dir), 0.0);

    vec3 diffuse = diff * LightColor * object_color * LightIntensity;

    // Specular component (highlights, reflections) - often simplified or removed in cel shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0); // 32.0 is shininess exponent
    vec3 specular = spec * LightColor * LightIntensity * 0.5; // specular color strength

    // Final lit color
    frag_color = vec4(ambient + diffuse + specular, 1.0);
}