#version 410 core

out vec4 FragColor;
in vec4 vertexColor;

uniform float time;

float sdfSphere(vec3 p, vec3 position, float radius)
{
    return length(p - position) - 1.0f;
}

float sdfBox( vec3 p, vec3 position, float b )
{
  vec3 q = abs(p - position) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float smoothMin(float a, float b, float k)
{
    float h = max(k - abs(a - b), 0.0f) / k;
    return min(a , b) - h*h*h*k*(1.0f/6.0f);
}

mat2 rot2d(float angle)
{
    float s = sin(angle);
    float c = cos(angle);
    return mat2(c, -s, s, c);
}

float scene(vec3 p)
{
    vec3 spherePos = vec3(sin(time), sin(time*0.27f + 2.0f), cos(time));
    float obj1 = sdfSphere(p, spherePos, 0.6f);
    p.yz *= rot2d(time * 0.2f);
    float obj2 = sdfBox(p, vec3(0.0f, 0.0f, 1.0f) , 0.5f);
    
    return smoothMin(obj1, obj2, 0.7f);
}

void main()
{
    vec2 uv = vertexColor.xy;
    
    vec3 origin = vec3(0.0f, 0.0f, -3.0f);
    vec3 direction = normalize(vec3(uv, 1.0f));
    
    float dist = 0.0f;
    
    vec3 stepcol;
    for (int i = 0; i < 80; i++)
    {
        vec3 position = origin + direction * dist;
        float march = scene(position);
        dist += march;
        
        stepcol = vec3(i) / 80.0f;
        if (march < 0.001f) break;
        if (dist > 100.0f) break;
    }
    
    vec3 distcol = vec3(clamp(1.0f / dist + 0.1f , 0.0f, 1.0f));
    
    FragColor = vec4(distcol * 1.0f + stepcol * 0.5f, 1.0f);
}
