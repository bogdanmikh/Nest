#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 TexCoord;
out vec3 Normal;
out vec3 Position;

void main() {
    vec4 pos = u_projection * u_view * u_model * vec4(aPos, 1.0);
    gl_Position = pos;
    //    gl_Position = u_model * vec4(aPos * 0.2, 1.0);
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(u_model))) * aNormal;
    Position = vec3(u_model * vec4(aPos, 1.0));
}