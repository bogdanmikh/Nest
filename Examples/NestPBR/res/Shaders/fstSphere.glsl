#version 330

in vec2 TexCoord;
in vec3 Normal;
in vec3 Position;

uniform float iTime;
uniform vec2 iResolution;
uniform vec2 iMouse;
uniform vec3 cameraPos;
uniform float metallic;
//uniform vec3 iColor;

uniform sampler2D iTexture;
uniform samplerCube iSky;

out vec4 fragColor;


void main() {
    vec2 st = gl_FragCoord.xy / iResolution;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    vec3 color = mix(texture(iTexture, TexCoord).rgb, texture(iSky, R).rgb, metallic);
    fragColor = vec4(color, 1.0);
}