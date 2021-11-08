#version 330 core

in VS_OUT{
    vec3 pos;
    vec3 normal;
} fs_in;

struct Light {
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;
uniform Light light;
uniform mat4 view;

out vec4 fragColor;

void main()
{
    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 n = normalize(fs_in.normal);
    vec3 light_pos = (view * vec4(light.pos, 1)).xyz;
    vec3 l = normalize(light_pos - fs_in.pos);
    float diff = max(dot(l, n), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    vec3 r = -reflect(l, n);
    vec3 v = normalize(vec3(0.0, 0.0, 0.0) - fs_in.pos);
    float spec = pow(max(dot(v, r), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    fragColor = vec4(ambient + diffuse + specular, 1.0);
}