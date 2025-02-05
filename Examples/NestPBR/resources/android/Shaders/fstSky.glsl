#version 300 es
in vec3 TexCoord;

out vec4 FragColor;

uniform samplerCube skyTexture;

void main() {
    FragColor = texture(skyTexture, TexCoord);
}