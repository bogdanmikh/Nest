#version 330
layout (location = 0) in vec3 aPos;

uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 TexCoord;

void main() {
    vec4 pos = u_projection * u_view * vec4(aPos * 20, 1.0);
//    gl_Position = pos.xyww;
    gl_Position = pos;
//    gl_Position = vec4(aPos, 1.0);
    TexCoord = aPos;
}