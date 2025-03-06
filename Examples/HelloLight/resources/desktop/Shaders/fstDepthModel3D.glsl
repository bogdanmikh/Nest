#version 330 core
out vec4 fragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec4 cameraPosVec4;
uniform vec4 cameraFrontVec4;

vec3 cameraPos = cameraPosVec4.rgb;
vec3 cameraFront = cameraFrontVec4.rgb;

float zNear = 0.1;
float zFar = 100.0;

float LinearizeDepth(float depth)  {
    // преобразуем обратно в NDC
    float z = depth * 2.0 - 1.0;
    return (2.0 * zNear * zFar) / (zFar + zNear - z * (zFar - zNear));
}

void main()  {
    // деление на zFar для лучшей наглядности
    float depth = LinearizeDepth(gl_FragCoord.z) / zFar;
    fragColor = vec4(vec3(depth), 1.0);
    // ortho
    // fragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}