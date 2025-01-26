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
    vec2 uv = fragCoord / iResolution.xy;
    vec2 resolution = iResolution.xy;

    // effect settings
    int countRect = 40;
    float intensiveEffect = 0.03;

    vec4 color = texture(iChannel0, uv);
    for (int i = 0; i < countRect; i++) {
        float rectWidth = 0.1 + 0.2 * float(i) / float(countRect);
        float rectHeight = 0.1 + 0.1 * float(i % 5) / float(countRect) + 0.01 * float(i % 3);

        float posX = float(i) / float(countRect) * resolution.x * 0.9;
        float posY = (sin(float(i))) * resolution.y * 0.9;

        if (fragCoord.x > posX && fragCoord.x < posX + rectWidth * resolution.x &&
        fragCoord.y > posY && fragCoord.y < posY + rectHeight * resolution.y) {

           // color.r = texture( iChannel0, vec2(uv.x + intensiveEffect, uv.y) ).r;
            color.g = texture( iChannel0, vec2(uv.x, uv.y) ).g;
            color.b = texture( iChannel0, vec2(uv.x - intensiveEffect, uv.y) ).b;
        }
    }
    fragColor = color;
}