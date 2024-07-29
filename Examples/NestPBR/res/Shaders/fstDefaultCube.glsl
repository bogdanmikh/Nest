#version 330

in vec2 TexCoord;

uniform float iTime;
uniform vec2 iResolution;
uniform vec2 iMouse;
//uniform vec3 iColor;

uniform sampler2D iTexture;

out vec4 fragColor;

void main() {
    vec2 st = gl_FragCoord.xy / iResolution;
    fragColor = texture(iTexture, TexCoord);
}