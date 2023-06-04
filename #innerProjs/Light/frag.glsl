#version 330 core

in vec4 vert;
out vec4 color;

struct Light{
    vec4 color;
    float range;
    float loss;
    float multi;
    float floating;
};

uniform Light l;
uniform sampler2D shTex;
uniform vec4 globalLightColor;
uniform float globalMul;
uniform float timing;

#define TIAO 6.2831852

vec4 calculateColor(Light lt,float len,float loss,float multi){
    float finalLoss = loss + timing * l.floating;
    vec4 rcolor = multi * clamp(finalLoss-len,0,1) * lt.color + clamp(1+len-finalLoss,0,1) * globalLightColor;
    return rcolor;
}

void main(){
    vec2 minus = vert.yx - vec2(l.range,l.range);
    vec4 pixel = vec4(1,0,0,0);
    float lens = length(minus) / l.range;
    color = globalMul * ((lens > pixel.x)?(globalLightColor):calculateColor(l,lens,l.loss,l.multi));
}
