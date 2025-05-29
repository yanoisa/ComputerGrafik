#version 330 core
in vec3 FragColor; // Received from vertex shader

out vec4 FragColorOut; // Output color

void main()
{
    FragColorOut = vec4(FragColor, 1.0); // Output the point's color with full alpha
}