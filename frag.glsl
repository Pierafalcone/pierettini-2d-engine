#version 410 core

out vec4 color;

in vec2 uv;

uniform sampler2D tex_base_color;

void main()
{
    vec3 base_color = texture(tex_base_color, vec2(uv.x, 1 - uv.y)).xyz;
    color = vec4(base_color, 1);
}