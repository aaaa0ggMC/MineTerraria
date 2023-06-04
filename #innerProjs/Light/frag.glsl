#version 330 core

in vec4 vert;
out vec4 color;

struct Light{
    vec2 position;
    vec4 color;
    float range;
};

uniform Light l;
uniform sampler2D shTex;
uniform vec4 globalLightColor;
uniform float globalMul;

#define TIAO 6.2831852

vec4 calculateColor(Light lt,float len){
    vec4 rcolor = lt.color * (1.0-len)+ globalLightColor * len;
    return rcolor;
}

void main(){
    ///计算好minus向量
    vec2 minus = vert.yx - vec2(l.range,l.range);
   // float pos = asin(normalize(minus).x) / TIAO;
   // pos = (minus.y>0)?pos:(1 - pos);
vec4 pixel = vec4(1,0,0,0);
    float lens = length(minus) / l.range;
    //color = globalMul * ((lens > pixel.x)?(globalLightColor):calculateColor(l,lens));
    color = globalMul * ((lens > pixel.x)?(globalLightColor):calculateColor(l,lens));
}
