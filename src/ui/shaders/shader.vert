#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 normal_matrix;
uniform mat4 projection;

out VS_OUT {
    vec3 pos;
    vec3 normal;
} vs_out;

void main()
{
    vs_out.pos = (view * vec4(in_pos, 1.0)).xyz;
    vs_out.normal = mat3(normal_matrix) * in_normal;

    gl_Position = projection * view * model * vec4(in_pos, 1.0);
}