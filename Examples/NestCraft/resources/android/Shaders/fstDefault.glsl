#version 300 es

uniform vec4 iTimeVec4;
uniform vec4 iResolutionVec4;
uniform vec4 iMouseVec4;
uniform sampler2D iChannel0;

float iTime = iTimeVec4.x;
vec2 iResolution = iResolutionVec4.xy;
vec2 iMouse = iMouseVec4.xy;


out vec4 f_ColorOut;

void mainImage( out vec4 fragColor, in vec2 fragCoord );

void main() {
    mainImage( f_ColorOut, gl_FragCoord.xy );
}

void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
    vec2 uv = fragCoord.xy / iResolution.xy;

    fragColor = texture(iChannel0, uv);
}