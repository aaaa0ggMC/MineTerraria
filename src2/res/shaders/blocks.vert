#version 330 core

layout(location = 0) in vec4 vert;

uniform float layer;

void main(){
    vec4 pixel = vert;
    pixel.z = layer;
    gl_Position = gl_ModelViewProjectionMatrix * pixel;
}
