#version 330

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
    float dispersion = .01;
    float distortion = .04;
    float noisestrength = .2;
    float bendscale = 1.5;

    vec2 uv = fragCoord.xy / iResolution.xy;
    vec2 disp = uv - vec2(.5, .5);
    disp *= sqrt(length(disp));
    uv += disp * bendscale;
    uv = (uv + .5)/2.0;
    vec2 uvr = uv * (1.0 - dispersion) + vec2(dispersion, dispersion)/2.0;
    vec2 uvg = uv * 1.0;
    vec2 uvb = uv * (1.0 + dispersion) - vec2(dispersion, dispersion)/2.0;

    vec3 offset = texture(iChannel0, vec2(0, uv.y + iTime * 255.0)).xyz;

    float r = mix(texture(iChannel0, vec2(uvr.x, uvr.y) + offset.x * distortion).xyz,
                  offset, noisestrength).x;
    float g = mix(texture(iChannel0, vec2(uvg.x, uvg.y) + offset.x * distortion).xyz,
                  offset, noisestrength).y;
    float b = mix(texture(iChannel0, vec2(uvb.x, uvb.y) + offset.x * distortion).xyz,
                  offset, noisestrength).z;

    if (uv.x > 0.0 && uv.x < 1.0 && uv.y > 0.0 && uv.y < 1.0) {
        float stripes = sin(uv.y * 300.0 + iTime * 10.0);
        vec3 col = vec3(r, g, b);
        col = mix(col, vec3(.8), stripes / 20.0);
        fragColor = vec4(col, 1.0);
    } else {
        fragColor = vec4(0, 0, 0, 1);
    }
}