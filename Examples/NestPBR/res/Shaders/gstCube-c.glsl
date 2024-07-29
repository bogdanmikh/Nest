#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 8) out;

in vec2 vTexCoord[];
in vec3 vNormal[];
in vec3 vPosition[];

out vec2 TexCoord;
out vec3 Normal;
out vec3 Position;

vec3 GetNormal() {
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {
    for (int i = 0; i < 8; i++) {
        gl_Position = gl_in[i].gl_Position;
        TexCoord = vTexCoord[i];
        Normal = vNormal[i];
        Position = vPosition[i];
        EmitVertex();
    }
    EndPrimitive();
}