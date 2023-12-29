#version 460 core


layout(binding = 0) uniform sampler2D tex;

out vec4 color;
in vec2 coord;


void main(){
    vec4 pick = texture2D(tex,coord.xy);
    if(pick == vec4(0))color = vec4(1.0);
    else color = pick;
}
