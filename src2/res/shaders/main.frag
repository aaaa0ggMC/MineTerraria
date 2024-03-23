#version 460 core
#extension GL_EXT_gpu_shader4 : enable


layout(binding = 0) uniform sampler2DArray tex;

out vec4 color;
in vec2 coord;
in flat int depth;

void main(){
    vec4 pick = texture2DArray(tex,vec3(coord,depth));
    color = pick;
}
