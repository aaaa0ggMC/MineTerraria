#version 460 core


layout(binding = 0) uniform sampler2D tex;

out vec4 color;
in vec2 coord;


void main(){
    vec4 pick = texture2D(tex,coord);
    color = pick;
}
