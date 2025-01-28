#version 330 core

in vec3 TexCoord;

out vec4 FragColor;

uniform sampler2D skyTexture;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)  {
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
    vec2 uv = SampleSphericalMap(normalize(TexCoord)); // make sure to normalize localPos
    uv.y = -uv.y;
    vec3 color = texture(skyTexture, uv).rgb;

    FragColor = vec4(color, 1.0);
}