#version 330

uniform vec4 iTimeVec4;
uniform vec4 iResolutionVec4;
uniform vec4 iMouseVec4;

float iTime = iTimeVec4.x;
vec2 iResolution = iResolutionVec4.xy;
vec2 iMouse = iMouseVec4.xy;

out vec4 fragColor;

float distortion = 0.01;      // the bias of the barrel distortion
const float iterations = 20.0;  // how many samples to use for edge blur
float strength = 0.2;      // how much edge blur is applied (to obscure the r, g, b separation)
float separation = 0.65;      // how much to separate the r, g and b

vec4 Aberrate(sampler2D source, vec2 texuv, vec2 squareUv, float amount) {
    return texture(source, 0.5 + texuv / sqrt(1.0 + amount * dot(texuv, squareUv)));
}

vec4 fragColor = gl_FragCoord;

void main() {
    vec2 v_TexCoordOut = fragCoord.xy / iResolution.xy;
    vec2 squareUv = v_TexCoordOut - 0.5;
    vec2 uv = v_TexCoordOut - 0.5;

    squareUv.x *= 0.3; // change radius
    squareUv.y *= 0.3; // change radius

    vec4 result = vec4(0, 0, 0, 1);

    for (float i = -iterations; i < iterations; i++) {
        float aberrateR = Aberrate(iChannel0, uv, squareUv, i * strength + (distortion + separation)).r;
        float aberrateG = Aberrate(iChannel0, uv, squareUv, i * strength + distortion).g;
        float aberrateB = Aberrate(iChannel0, uv, squareUv, i * strength + (distortion - separation)).b;
        //  result.r += aberrateR;
        result.g += aberrateG;
        result.b += aberrateB;
    }

    result.rgb /= iterations * 2.0;

    vec4 originalColor = texture(iChannel0, v_TexCoordOut);

    if (result.g + result.b > 0.0) {
        fragColor = vec4(originalColor.r, result.g, result.b, originalColor.a);
    } else {
        fragColor = originalColor;
    }
}