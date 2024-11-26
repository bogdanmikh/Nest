#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 projViewMtx;

out vec3 Position;
out vec2 TexCoord;
out vec3 Normal;

void main() {
    vec4 pos = projViewMtx * model * vec4(aPos, 1.0);
    gl_Position = pos;
//    gl_Position = vec4(aPos, 1.0);
//    gl_Position = model * vec4(aPos * 0.2, 1.0);
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
}