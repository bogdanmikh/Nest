#version 330 core
out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D model3D;

void main() {
//    fragColor = texture(model3D, TexCoords);
    fragColor = vec4(1.);
}