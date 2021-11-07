#version 330 core

in VS_OUT{
    vec3 pos;
    vec4 color;
    vec3 normal;
} fs_in;

uniform mat4 view;

out vec4 fragColor;

void main()
{
    vec3 n = normalize(fs_in.normal);

    vec3 light_pos = (view * vec4(0, 50, 0, 1)).xyz;
    vec3 l = normalize(light_pos - fs_in.pos);

    vec3 r = -reflect(l, n);
    vec3 v = normalize(vec3(0.0, 0.0, 0.0) - fs_in.pos);

    float ambient = 0.1;
    float diffuse = 0.7 * max(dot(l, n), 0.0);
    float specular = 0.6 * pow (max(dot(v, r), 0.0), 32.0);
    fragColor = fs_in.color * (ambient + diffuse + specular);
}