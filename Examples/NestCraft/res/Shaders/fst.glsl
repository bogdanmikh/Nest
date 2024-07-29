#version 330

in vec2 TexCoord;
in float Light;

uniform float u_time;
uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform vec3 u_color;

uniform sampler2D texture1;

out vec4 fragColor;

void main() {
    vec2 st = gl_FragCoord.xy / u_resolution;
    vec2 m = gl_FragCoord.xy / u_mouse;
    fragColor = vec4(Light * u_color.x, Light * u_color.y, Light * u_color.z, 1.) * texture(texture1, TexCoord);
}