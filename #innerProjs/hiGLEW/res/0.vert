#version 430

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texc;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform float tf;
uniform mat4 cr_matrix;

out vec4 vcolor;
out vec2 coord;

mat4 rotateX(float a);
mat4 rotateY(float a);
mat4 rotateZ(float a);
mat4 translate(float x,float y,float z);
mat4 scale(float x,float y,float z);

void main(){
    float i = gl_InstanceID + tf;
    float a = sin(2*i) * 8;
    float b = sin(3*i) * 8;
    float c = sin(4*i) * 8;

    mat4 x = rotateX(i);
    mat4 y = rotateY(i);
    mat4 z = rotateZ(i);

    mat4 trans = translate(a,b,c);

    mat4 mv_matrix = v_matrix * m_matrix;// * x * y * z;

    vec4 rpos = mv_matrix * vec4(pos,1.0);
    rpos *= cr_matrix;
    rpos = proj_matrix * rpos;
    gl_Position = rpos;
    vcolor = vec4(pos,1.0) * 0.5 + vec4(0.5,0.5,0.5,0.5);
    coord = texc;
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
