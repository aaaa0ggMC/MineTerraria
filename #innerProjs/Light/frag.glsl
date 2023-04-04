#version 330 core

in vec4 vert;
out vec4 color;

void main(){
    color = vec4(dot(normalize(vert),vec4(0,0,1,1.0)));
    color.a = 1.0;
}
