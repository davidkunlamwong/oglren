#version 410 core

out vec4 FragColor;
in vec4 vertexColor;

uniform float time;

float sinNorm(float var, float freqHz, float phaseNorm)
{
    return (sin(2*3.14*(freqHz*var+phaseNorm))+1.0f)/2.0f;
}

void main()
{
    vec2 uv = (vertexColor.xy + 1.0f) / 2.0f;
    vec3 col = vec3(sinNorm(time + vertexColor.y, 1.0f, 0.0f),
                    sinNorm(time + 0.1f * vertexColor.y + 0.144 * vertexColor.x, 1.7f, 0.333f),
                    sinNorm(time + vertexColor.x, 1.3f, 0.667f));
    FragColor = vec4(col, 1.0f);
}
