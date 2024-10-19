#version 330 core

out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D tex;

void main() {
     fragColor = texture(tex, TexCoords);
    //fragColor = vec4(vec3(0.1, 0.3, 0.2), 1.0);
}