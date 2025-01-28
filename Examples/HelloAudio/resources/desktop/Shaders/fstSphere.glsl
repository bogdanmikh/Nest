#version 330

in vec2 TexCoord;
in vec3 Normal;
in vec3 Position;

uniform vec4 iTimeVec4;
uniform vec4 iResolutionVec4;
uniform vec4 iMouseVec4;
uniform vec4 iCameraPosVec4;

float iTime = iTimeVec4.x;
vec2 iResolution = iResolutionVec4.xy;
vec2 iMouse = iMouseVec4.xy;
vec3 cameraPos = iCameraPosVec4.xyz;

//uniform vec3 iColor;

uniform sampler2D iTexture;
uniform samplerCube iSky;

out vec4 fragColor;

void main() {
    vec2 st = gl_FragCoord.xy / iResolution;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    vec3 color = texture(iTexture, TexCoord).rgb;
//    vec3 color = mix(texture(iTexture, TexCoord).rgb, texture(iSky, R).rgb, metallic);
    fragColor = vec4(color, 1.0);
}