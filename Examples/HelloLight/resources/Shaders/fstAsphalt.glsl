#version 330 core

out vec4 fragColor;

in vec2 TexCoords;
in vec3 Normal;

uniform sampler2D texture_diffuse;
uniform vec4 vec4LightPos;
uniform vec4 vec4LightColor;
vec3 lightPos = vec4LightPos.rgb;
vec3 lightColor = vec4LightColor.rgb;

void main() {
    float ambientStrength = 1.f;
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(lightPos - Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 objectColor = texture(texture_diffuse, TexCoords).rgb;
    vec3 result = (ambientStrength + diffuse) * objectColor;
    fragColor = vec4(result * ambientStrength * lightColor.r, 1.0);
   // fragColor = vec4(vec3(0.7, 0.7, 0.7), 1.0);
}