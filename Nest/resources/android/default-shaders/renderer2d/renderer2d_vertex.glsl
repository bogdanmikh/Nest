#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in float textureIndex;
layout (location = 3) in vec4 color;

uniform mat4 projViewMtx;

out float fragTextureIndex;
out vec4 fragColor;
out vec2 fragTexCoord;

void main(){
    fragTextureIndex = textureIndex;
    fragColor = color;
    fragTexCoord = texCoord;
    gl_Position = projViewMtx * vec4(position, 1);
}
