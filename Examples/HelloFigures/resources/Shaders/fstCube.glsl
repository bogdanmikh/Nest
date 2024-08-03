#version 330

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

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

out vec4 fragColor;

float metalic = 0.3;

void main() {
    vec2 st = gl_FragCoord.xy / iResolution;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    vec3 color = texture(iTexture, TexCoord).rgb;
//    fragColor = vec4(1., 0., 1., 1.);
    fragColor = vec4(color, 1.0);
}