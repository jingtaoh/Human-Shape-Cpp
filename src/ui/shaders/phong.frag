#version 330 core

in VS_OUT{
    vec3 pos;
    vec3 normal;
    vec4 color;
} fs_in;

struct Light {
    vec3 dir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform mat4 view;

out vec4 fragColor;

void main()
{
    // ambient
    vec3 ambient = light.ambient;

    // diffuse
    vec3 n = normalize(fs_in.normal);
    vec3 l = normalize((view * vec4(-light.dir,0)).xyz);
    float diff = max(dot(l, n), 0.0);
    vec3 diffuse = light.diffuse * diff;

    // specular
    vec3 r = -reflect(l, n);
    vec3 v = normalize(vec3(0.0, 0.0, 0.0) - fs_in.pos);
    float spec = pow(max(dot(v, r), 0.0), 32.f);
    vec3 specular = light.specular * spec;

    fragColor = fs_in.color * vec4(ambient + diffuse + specular, 1.0);
}