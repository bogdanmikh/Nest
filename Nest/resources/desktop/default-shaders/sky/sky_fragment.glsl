#version 330 core

in vec3 TexCoord;

out vec4 FragColor;

uniform samplerCube skyTexture;

void main()
{
	FragColor = texture(skyTexture, TexCoord);
}
