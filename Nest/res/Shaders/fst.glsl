#version 330

in vec2 TexCoord;

uniform float u_time;
uniform vec2 u_resolution;
uniform vec2 u_mouse;

uniform sampler2D texture1;

out vec4 fragColor;

void main() {
    vec2 st = gl_FragCoord.xy / u_resolution;
    vec2 m = gl_FragCoord.xy / u_mouse;
    float r = 0.5f, g = 0.f, b = 1.f;
    fragColor = vec4(r, g, b, 1.) * texture(texture1, TexCoord);
}