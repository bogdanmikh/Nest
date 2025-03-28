#version 330 core

out vec4 fragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    vec4 shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

struct DirLightVec4 {
    vec4 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct PointLightVec4 {
    vec4 position;

    vec4 constant;
    vec4 linear;
    vec4 quadratic;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
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
#define MAX_SPOT_LIGHTS 1

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

// x = dirLights, y = pointLights, z = spotLights
uniform vec4 numLights;
uniform DirLightVec4 dirLightsVec4[MAX_DIR_LIGHTS];
uniform PointLightVec4 pointLightsVec4[MAX_POINT_LIGHTS];
uniform SpotLightVec4 spotLightsVec4[MAX_SPOT_LIGHTS];
uniform Material material;
uniform sampler2D shadowMap;

uniform vec4 cameraPosVec4;
uniform vec4 cameraFrontVec4;

DirLight dirLights[MAX_DIR_LIGHTS];
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

int numDirLights = int(numLights.x);
int numPointLights = int(numLights.y);
int numSpotLights = int(numLights.z);

vec3 cameraPos = cameraPosVec4.rgb;
vec3 cameraFront = cameraFrontVec4.rgb;
float shadow;

void initLights();
vec3 calcDirLight(DirLight light, vec3 normal, vec3 cameraDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 cameraDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 cameraDir);
float shadowCalculation(vec4 fragPosLightSpace);

float gamma = 1.2;

void main() {
    initLights();
    vec3 norm = normalize(Normal);
    vec3 cameraDir = normalize(cameraPos - FragPos);
//    shadow = shadowCalculation(FragPosLightSpace);
    shadow = 0;

    vec3 result = vec3(0);
    for(int i = 0; i < min(numDirLights, MAX_DIR_LIGHTS); i++) {
        result += calcDirLight(dirLights[i], norm, cameraDir);
    }

    for(int i = 0; i < min(numPointLights, MAX_POINT_LIGHTS); i++) {
        result += calcPointLight(pointLights[i], norm, FragPos, cameraDir);
    }

    for(int i = 0; i < min(numSpotLights, MAX_SPOT_LIGHTS); i++) {
        result += calcSpotLight(spotLights[i], norm, FragPos, cameraDir);
    }

    result = pow(result, vec3(1.0 / gamma));
    fragColor = vec4(result, 1);
}

void initLights() {
    // dir light
    for(int i = 0; i < min(numDirLights, MAX_DIR_LIGHTS); i++) {
        dirLights[i].specular = dirLightsVec4[i].specular.rgb;
        dirLights[i].diffuse = dirLightsVec4[i].diffuse.rgb;
        dirLights[i].direction = dirLightsVec4[i].direction.rgb;
        dirLights[i].ambient = dirLightsVec4[i].ambient.rgb;
    }

    // point light
    for(int i = 0; i < min(numPointLights, MAX_POINT_LIGHTS); i++) {
        pointLights[i].position = pointLightsVec4[i].position.rgb;
        pointLights[i].constant = pointLightsVec4[i].constant.r;
        pointLights[i].linear =  pointLightsVec4[i].linear.r;
        pointLights[i].quadratic = pointLightsVec4[i].quadratic.r;
        pointLights[i].ambient = pointLightsVec4[i].ambient.rgb;
        pointLights[i].diffuse = pointLightsVec4[i].diffuse.rgb;
        pointLights[i].specular = pointLightsVec4[i].specular.rgb;
    }

    // spot
    for(int i = 0; i < min(numSpotLights, MAX_SPOT_LIGHTS); i++) {
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

// calculates the color when using a directional light.
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)  {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess.r);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + (1. - shadow) * (diffuse + specular));
}

// calculates the color when using a point light.
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)  {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess.r);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + (1. - shadow) * (diffuse + specular));
}

// calculates the color when using a spot light.
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
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
    return (ambient + (1. - shadow) * (diffuse + specular));
}

float shadowCalculation(vec4 fragPosLightSpace) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    return shadow;
}