#version 330 core

out vec4 fragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    vec4 shininess;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform vec4 cameraPosVec4;
uniform vec4 cameraFrontVec4;
uniform sampler2D diffuse;

vec3 cameraPos = cameraPosVec4.rgb;
vec3 cameraFront = cameraFrontVec4.rgb;

void main() {
    fragColor = texture(diffuse, TexCoords);
}