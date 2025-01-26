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

#define PIXEL_SIZE 2.0

void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
    vec2 uv = fragCoord.xy / iResolution.xy;

    float plx = iResolution.x * PIXEL_SIZE / 500.0;
    float ply = iResolution.y * PIXEL_SIZE / 275.0;

    float dx = plx * (1.0 / iResolution.x);
    float dy = ply * (1.0 / iResolution.y);

    uv.x = dx * floor(uv.x / dx);
    uv.y = dy * floor(uv.y / dy);

    fragColor = texture(iChannel0, uv);
}
