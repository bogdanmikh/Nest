#version 330

uniform vec4 iTimeVec4;
uniform vec4 iResolutionVec4;
uniform vec4 iMouseVec4;
uniform sampler2D iChannel0;

uniform vec4 nearPlaneVec4;
uniform vec4 farPlaneVec4;

float iTime = iTimeVec4.x;
vec2 iResolution = iResolutionVec4.xy;
vec2 iMouse = iMouseVec4.xy;
float nearPlane = nearPlaneVec4.x;
float farPlane = farPlaneVec4.x;

out vec4 f_ColorOut;

void mainImage( out vec4 fragColor, in vec2 fragCoord );

void main() {
    mainImage( f_ColorOut, gl_FragCoord.xy );
}

// required when using a perspective projection matrix
float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
    vec2 uv = fragCoord.xy / iResolution.xy;
    float depthValue = texture(iChannel0, uv).r;
    // fragColor = vec4(vec3(LinearizeDepth(depthValue) / farPlane), 1.0); // perspective
    fragColor = vec4(vec3(depthValue), 1.0); // orthographic
}