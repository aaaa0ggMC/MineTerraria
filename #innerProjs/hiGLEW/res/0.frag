#version 430

layout(binding = 0) uniform sampler2D tex;
layout(location = 3) in vec3 norm;

out vec4 color;
in vec2 coord;
in vec3 fpos;
in vec3 norm;

//0:no texture,1 texure
uniform float blend;

struct DotLight{
    vec4 color;
    vec4 position;
};
struct Mesh{
    vec4 color;
};

uniform DotLight l;
uniform Mesh mesh;
uniform vec3 observer;

void main(){
//    color = vec4(coord.xy,0,1.0);
    vec4 pixel = vec4(norm+vec3(0.5,0.5,0.5),1.0) * (1.0 - blend) + texture(tex,coord) * blend;
    color = vec4(pixel.xyz,1.0);
}
