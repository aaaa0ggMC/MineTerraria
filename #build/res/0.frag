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
struct Material{
    vec4 color;
    float shiness;
};

uniform GlobalLight ambient;
uniform DotLight l;
uniform Material material;
uniform vec3 observer;

vec4 RenderDotLightBlinnPhong(DotLight,vec3,vec3,vec3,Material);

void main(){
    vec4 pixel = material.color * (1.0 - blend) + texture(tex,coord) * blend;
    //Ambient
    vec4 ambientColor = ambient.strength * ambient.color;
    vec3 normal = normalize(norm);

    pixel *= (ambientColor + RenderDotLightBlinnPhong(l,observer,fpos,normal,material));
    color = vec4(pixel.xyz,1.0);
}

vec4 RenderDotLightBlinnPhong(DotLight ls,vec3 obs,vec3 fragPos,vec3 normal,Material m){
     //Diffuse
    vec3 lightDir = normalize(ls.position - fragPos);
    float diff = max(dot(normal,lightDir),0);
    vec4 diffuse = diff * ls.color;
    //Specular
    vec3 halfVector = lightDir - fragPos;
    float spec = pow(max(dot(normal,halfVector),0),material.shiness);
    vec4 specular = ls.strength * spec * ls.color;
    return (diffuse + specular) * ls.strength * 0.5;
}
