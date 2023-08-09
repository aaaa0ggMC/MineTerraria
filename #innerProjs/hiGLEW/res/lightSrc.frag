#version 430

out vec4 color;

struct GlobalLight{
    vec4 color;
    float strength;
};

uniform GlobalLight gcolor;

void main(){
    color = gcolor.strength * gcolor.color;
}
