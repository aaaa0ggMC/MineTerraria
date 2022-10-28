#version 400

#extension GL_EXT_gpu_shader4 : enable

uniform sampler2D cloudTex;

void main(){
    vec4 pixel = texture2D(cloudTex,gl_TexCoord[0].xy);
    gl_FragColor = pixel;
}
