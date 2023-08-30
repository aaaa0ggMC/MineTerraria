#version 430
layout(location = 0) in vec3 pos;

uniform mat4 vrp_matrix;
uniform mat4 m_matrix;

out vec4 vertv;

void main(){
    gl_Position = vrp_matrix * m_matrix * vec4(pos,1.0);
}

