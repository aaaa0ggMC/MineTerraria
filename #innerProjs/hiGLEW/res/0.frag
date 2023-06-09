#version 430

uniform sampler2D tex;

out vec4 color;
in vec2 coord;

void main(){
//    color = vec4(coord.xy,0,1.0);
    color = texture(tex,coord);
}
