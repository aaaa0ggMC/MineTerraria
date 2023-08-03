#version 430

layout(binding = 0) uniform sampler2D tex;

out vec4 color;
in vec2 coord;
in vec4 vcolor;

//0:no texture,1 texure
uniform float blend;

void main(){
//    color = vec4(coord.xy,0,1.0);
    vec4 pixel = vcolor * (1.0 - blend) + texture(tex,coord) * blend;
    color = vec4(pixel.xyz,1.0);
}
