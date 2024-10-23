#version 330 core

out vec4 fragColor;

in vec2 TexCoords;

uniform sampler2D tex;

// light
struct Light {
    vec4 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

void main() {
    vec3 lightDir = normalize(-light.direction.xyz);
    fragColor = texture(tex, TexCoords);
   // fragColor = vec4(vec3(0.7, 0.7, 0.7), 1.0);
}