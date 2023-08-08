#version 430

layout(binding = 0) uniform sampler2D tex;

out vec4 color;
in vec2 coord;
in vec3 fpos;
in vec3 norm;

//0:no texture,1 texure
uniform float blend;

struct GlobalLight{
    vec4 color;
    float strength;
};
struct DotLight{
    vec4 color;
    vec3 position;
    float strength;
};
struct Mesh{
    vec4 color;
};

uniform GlobalLight ambient;
uniform DotLight l;
uniform Mesh mesh;
uniform vec3 observer;

vec4 RenderDotLight(DotLight,vec3,vec3,vec3);

void main(){
    vec4 pixel = mesh.color * (1.0 - blend) + texture(tex,coord) * blend;
    //Ambient
    vec4 ambientColor = ambient.strength * ambient.color;
    vec3 normal = normalize(norm);

    pixel *= (ambientColor + RenderDotLight(l,observer,fpos,normal));
    color = vec4(pixel.xyz,1.0);
}

vec4 RenderDotLight(DotLight ls,vec3 obs,vec3 fragPos,vec3 normal){
     //Diffuse
    vec3 lightDir = normalize(ls.position - fragPos);
    float diff = max(dot(normal,lightDir),0);
    vec4 diffuse = diff * ls.color;
    //Specular
    vec3 viewDir = normalize(obs - fragPos);
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0),32);
    vec4 specular = ls.strength * spec * ls.color;

    return diffuse + specular;
}
