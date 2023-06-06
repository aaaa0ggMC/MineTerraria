#version 430

layout(location = 0) in vec3 pos;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec4 vcolor;

void main(){
    gl_Position = proj_matrix * mv_matrix * vec4(pos,1.0);
    vcolor = vec4(pos,1.0) * 0.5 + vec4(0.5,0.5,0.5,0.5);
}
