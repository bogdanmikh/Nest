#version 330 core

in vec4 Color;
out vec4 FragColor;

uniform vec4 iTime;

void main()
{
    FragColor = Color * abs(sin(iTime.x));
}
