#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 cd;

layout(location=1) uniform mat4 m_matrix;
layout(location=2) uniform mat4 vrp_matrix;

out vec2 coord;

void main(){
    gl_Position = vrp_matrix * m_matrix * vec4(pos,1.0);
    coord = cd;
}

