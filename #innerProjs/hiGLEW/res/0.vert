#version 430

uniform float off;

void main(){
    if(gl_VertexID == 0)gl_Position = vec4(0.25 + off,-0.25,0,1.0);
    else if(gl_VertexID == 1)gl_Position = vec4(-0.25 + off,-0.25,0,1.0);
    else gl_Position = vec4(0.25 + off,0.25,0,1.0);
}
