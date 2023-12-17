#version 400

#extension GL_EXT_gpu_shader4 : enable

uniform vec3 poses;
uniform vec3 target;
uniform vec2 extra;

uniform sampler2D tex;

float restrict_fun(float x,float minv,float maxv){
    return x<minv?minv:(x>maxv?maxv:x);
}

vec4 lookup(sampler2D texm,float x,float y){
    //if strict:.../target.z means .../(target.z - target.x) and so on
    vec2 coord = vec2(restrict_fun(x,poses.x,poses.z)/target.z,restrict_fun(y,poses.y,extra.x)/extra.y);
    return texture2D(texm,coord.xy);
}

#define blurLevel 3
float maxlen = blurLevel * 1.414213562373095;

void main(){
    //Look up 9 pixels around
    vec4 pixel ;
    for(int i = -blurLevel;i < blurLevel-1;i++){
        for(int j = -blurLevel;j < blurLevel-1;j++){
            //ChangeBlurLevelNeedsToChangeThis
            //这TM是人能犯的错误？？？！Is this mistake a human do???
            //pixel += lookup(tex,gl_FragCoord.xy)/49 * weight[i+1][j+1];
            //Linear Blur Testing
            //不需要对采样点除法，之后会将颜色正规化
            //如何节省性能：https://blog.csdn.net/zxcasd11/article/details/107710990
            pixel += lookup(tex,gl_FragCoord.x + i + 10,gl_FragCoord.y + j + 10) * sqrt(i*i + j*j)/maxlen;
        }
    }
    //vec2 pos0 = vec2(gl_FragCoord.x-10.0,gl_FragCoord.y);
    //vec4 pixel = lookup(tex,pos0.xy);
    //This step:We make the glass not black
    //And then multiply it with little bit black
    float percent = 0.95 / pixel.a;
    pixel *= percent;
    //不需要 gl_Color * pixel,不然会出现重影，这属于要用莎普爱思了！！
    gl_FragColor = pixel;
}
