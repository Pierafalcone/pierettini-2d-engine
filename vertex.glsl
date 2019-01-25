#version 410 core

layout(location=0) in vec3 vertex;
layout(location=1) in vec2 model_uv;

out vec2 uv;

void main()
{
    uv = normalize(model_uv);
    gl_Position = vec4(vertex.x, vertex.y, vertex.z, 1);
}