#version 410 core

out vec4 FragColor;
in vec4 vertexColor;

uniform float time;

void main()
{
   FragColor = vec4(fract(abs(vertexColor.rgb)+time), 1.0f);
}
