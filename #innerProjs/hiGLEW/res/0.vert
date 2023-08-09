#version 430

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texc;
layout(location = 2) in vec3 onorm;

uniform mat4 m_matrix;
uniform mat4 vrp_matrix;

out vec2 coord;
out vec3 fpos;
out vec3 norm;

mat4 rotateX(float a);
mat4 rotateY(float a);
mat4 rotateZ(float a);
mat4 translate(float x,float y,float z);
mat4 scale(float x,float y,float z);

void main(){
    gl_Position =  vrp_matrix * m_matrix * vec4(pos,1.0);
    coord = texc;
    norm = mat3(transpose(inverse(m_matrix))) * onorm;
    fpos = (m_matrix * vec4(pos,1.0)).xyz;
}

mat4 rotateX(float a){
    mat4 x = mat4(
       1.0,0.0,0.0,0.0,
       0.0,cos(a),-sin(a),0.0,
       0.0,sin(a),cos(a),0.0,
       0.0,0.0,0.0,1.0
    );
    return x;
}

mat4 rotateY(float a){
    mat4 y = mat4(
        cos(a),0.0,sin(a),0,
        0.0,1.0,0.0,0.0,
        -sin(a),0.0,cos(a),0.0,
        0.0,0.0,0.0,1.0
    );
    return y;
}

mat4 rotateZ(float a){
    mat4 z = mat4(
    cos(a),-sin(a),0,0,
    sin(a),cos(a),0,0,
    0,0,1,0,
    0,0,0,1
    );
    return z;
}

mat4 translate(float x,float y,float z){
    mat4 t = mat4(
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        x,y,z,1
    );
    return t;
}

mat4 scale(float x,float y,float z){
    mat4 s =  mat4(
        x,0,0,0,
        0,y,0,0,
        0,0,z,0,
        0,0,0,1
    );
    return s;
}
