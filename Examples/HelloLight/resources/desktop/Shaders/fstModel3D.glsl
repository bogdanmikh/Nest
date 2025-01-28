#version 330 core

out vec4 fragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

// light uniforms
uniform vec4 lightPosition;
uniform vec4 lightDirection;
uniform vec4 lightCutOff;
uniform vec4 lightOuterCutOff;
uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;
uniform vec4 lightConstant;
uniform vec4 lightLinear;
uniform vec4 lightQuadratic;

// material
uniform vec4 shininess;
// end material

uniform vec4 vec4CameraPos;

vec3 cameraPos = vec4CameraPos.rgb;

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    vec4 shininess;
};

Light light;

uniform Material material;

void updateLight() {
    light.position = lightPosition.rgb;
    light.direction = lightDirection.rgb;
    light.cutOff = lightCutOff.r;
    light.outerCutOff = lightOuterCutOff.r;

    light.ambient = lightAmbient.rgb;
    light.diffuse = lightDiffuse.rgb;
    light.specular = lightSpecular.rgb;

    light.constant = lightConstant.r;
    light.linear = lightLinear.r;
    light.quadratic = lightQuadratic.r;
}

void main() {
    updateLight();

    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // specular
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess.r);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    //    vec3 result = texture(texture_diffuse, TexCoords).rgb;
    fragColor = vec4(result, 1.0);
}