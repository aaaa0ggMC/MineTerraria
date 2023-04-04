#version 330 core

layout(location = 0) in vec4 verts;

out vec4 vert;

void main(){
    vert = verts;
    gl_Position = gl_ModelViewProjectionMatrix * verts;
    gl_FrontColor = gl_Color;
}
