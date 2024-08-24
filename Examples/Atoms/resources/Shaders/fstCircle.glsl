#version 330

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

uniform vec4 iTimeVec4;
uniform vec4 iResolutionVec4;
uniform vec4 iMouseVec4;
uniform vec4 iCameraPosVec4;
uniform vec4 iColorVec4;

float iTime = iTimeVec4.x;
vec2 iResolution = iResolutionVec4.xy;
vec2 iMouse = iMouseVec4.xy;
vec3 cameraPos = iCameraPosVec4.xyz;
vec3 iColor = iColorVec4.xyz;

uniform sampler2D iTexture;

out vec4 fragColor;

float metalic = 0.3;

void main() {
    vec2 st = TexCoord;
    vec4 color = texture(iTexture, st);
    color.xyz *= iColor;
    fragColor = color;
}