#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

out vec4 fragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    vec4 shininess;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLightVec4 {
    vec4 position;
    vec4 direction;
    vec4 cutOff;
    vec4 outerCutOff;

    vec4 constant;
    vec4 linear;
    vec4 quadratic;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

#define MAX_DIR_LIGHTS 1
#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10

// x = dirLights, y = pointLights, z = spotLights
uniform vec4 numLights;
uniform SpotLightVec4 spotLightsVec4[MAX_SPOT_LIGHTS];
uniform Material material;
uniform sampler2D shadowMap;

uniform vec4 cameraPosVec4;
uniform vec4 cameraFrontVec4;

SpotLight spotLights[MAX_SPOT_LIGHTS];

int numDirLights = int(numLights.x);
int numPointLights = int(numLights.y);
int numSpotLights = int(numLights.z);

vec3 cameraPos = cameraPosVec4.rgb;
vec3 cameraFront = cameraFrontVec4.rgb;

void initLights();
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 cameraDir, float shadow);
float shadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);

float gamma = 1.2;

void main() {
    vec3 norm = normalize(Normal);
    vec3 cameraDir = normalize(cameraPos - FragPos);
    vec3 lightDir = normalize(spotLights[0].position - FragPos);

    initLights();
    float shadow = shadowCalculation(FragPosLightSpace, norm, lightDir);
//    shadow = abs(shadow - 1);
    fragColor = vec4(vec3(shadow), 1.);
//    return;
    vec3 result = vec3(0);

    for(int i = 0; i < min(numSpotLights, MAX_SPOT_LIGHTS); i++) {
        result += calcSpotLight(spotLights[i], norm, FragPos, cameraDir, shadow);
    }

//    result = pow(result, vec3(1.0 / gamma));
    fragColor = vec4(result, 1);
}

void initLights() {
    // spot
    for (int i = 0; i < min(numSpotLights, MAX_SPOT_LIGHTS); i++) {
        spotLights[i].position = spotLightsVec4[i].position.rgb;
        spotLights[i].direction = spotLightsVec4[i].direction.rgb;
        spotLights[i].cutOff = spotLightsVec4[i].cutOff.r;
        spotLights[i].outerCutOff = spotLightsVec4[i].outerCutOff.r;
        spotLights[i].constant = spotLightsVec4[i].constant.r;
        spotLights[i].linear = spotLightsVec4[i].linear.r;
        spotLights[i].quadratic = spotLightsVec4[i].quadratic.r;
        spotLights[i].ambient = spotLightsVec4[i].ambient.rgb;
        spotLights[i].diffuse = spotLightsVec4[i].diffuse.rgb;
        spotLights[i].specular = spotLightsVec4[i].specular.rgb;
    }
}

// calculates the color when using a spot light.
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess.r);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (1 - shadow) * (ambient + (diffuse + specular));
}

float shadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    if (currentDepth > 1.0) {
        return 0.0;
    }
    // check whether current frag pos is in shadow
//    float bias = 0.005;
    float minBias = 0.005;
    float maxBias = 0.05;
    float bias = max(maxBias * (1.0 - dot(normal, lightDir)), minBias);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}