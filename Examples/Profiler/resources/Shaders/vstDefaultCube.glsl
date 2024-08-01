#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 TexCoord;

void main() {
    vec4 pos = u_projection * u_view * u_model * vec4(aPos, 1.0);
    gl_Position = pos;
    //    gl_Position = u_model * vec4(aPos * 0.2, 1.0);
    TexCoord = aTexCoord;
}