#version 400

uniform sampler2D texture;
uniform int time;

#extension GL_EXT_gpu_shader4 : enable

float restrict_fun(float x,float minv,float maxv){
    return x<minv?minv:(x>maxv?maxv:x);
}

float reOverride(float x){
    return (x - int(x))>0?x - int(x):1-int(x) + x;
}

#define PIXEL_GRANULARITY 16

void main(){
    vec2 lookUpPos = gl_TexCoord[0].xy;
    bool re = (time+3500) % 5000 >= 4000;
    lookUpPos.x += (int(gl_FragCoord.x) % PIXEL_GRANULARITY) >= PIXEL_GRANULARITY/2?0.01f:-0.01f;
    lookUpPos.y += (int(gl_FragCoord.y) %PIXEL_GRANULARITY) >= PIXEL_GRANULARITY/2?0.01f:-0.01f;
    lookUpPos.x = reOverride(restrict_fun(lookUpPos.x,0,1) + (((gl_FragCoord.y >= 200) && (gl_FragCoord.y <= 400))?-0.3f:0));
    lookUpPos.y = restrict_fun(lookUpPos.y,0,1);
    vec4 pixel = texture2D(texture,re?lookUpPos.xy:gl_TexCoord[0].xy);
    float aver = re?(0.2126 * pixel.r + 0.0722*pixel.b + 0.7152*pixel.g):0;
    pixel.r = re?aver:pixel.r;
    pixel.g = re?aver:pixel.g;
    pixel.b = re?aver:pixel.b;
    gl_FragColor = gl_Color * pixel;
}
