#version 410 core

layout(location=0) in vec3 vertex;
layout(location=1) in vec2 model_uv;

out vec2 uv;

uniform vec2 position;
uniform vec2 window_size;
uniform float scale;

void main()
{
    uv = model_uv;

    vec2 pixel_unit = (1 / window_size) * 2;

    vec2 quad_pos = vec2(vertex.x, vertex.y);
    vec2 scaled_pos = (quad_pos / window_size) * scale;
    vec2 final_pos = scaled_pos + (pixel_unit * position);

    final_pos -= 1;

    final_pos.y *= -1;

    gl_Position = vec4(final_pos, vertex.z, 1);
}