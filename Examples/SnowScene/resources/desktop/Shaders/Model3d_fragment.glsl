#version 330 core

out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D diffuse;

void main() {
    fragColor = texture(diffuse, TexCoords);
   // fragColor = vec4(vec3(0.7, 0.7, 0.7), 1.0);
}