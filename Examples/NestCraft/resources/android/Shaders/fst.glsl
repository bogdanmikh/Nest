#version 300 es

in vec2 TexCoord;
in float Light;

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

uniform sampler2D texture1;

out vec4 fragColor;

void main() {
    vec2 st = gl_FragCoord.xy / iResolution;
    vec2 m = gl_FragCoord.xy / iMouse;

    vec4 res = vec4(Light * iColor.x, Light * iColor.y, Light * iColor.z, 1.) * texture(texture1, TexCoord);
    fragColor = res;
}