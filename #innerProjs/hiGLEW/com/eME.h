#ifndef EME_INCL
#define EME_INCL

#include "ME.h"
#define numVBOs 32
#define numVAOs 1

namespace me{
    struct eApp2D{
        eApp2D();
        ~eApp2D();

        int init(unsigned int width,unsigned int height,std::string title,Window::OnKeyPress);

        bool Running();
        void Display();

        Window window;
        VBOs vbo;
        Program game;
        GLuint vao[numVAOs];

        Camera camera{0,0,0};
        Velocity camSpeed{10};

        GObject templatePlane{0,0,0};

        ///default 2d renderer
        Shader shader{false};

        constexpr const static char * def_vert =
R"(#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 cd;

layout(location=1) uniform mat4 m_matrix;
layout(location=2) uniform mat4 vrp_matrix;

out vec2 coord;

void main(){
    gl_Position = vrp_matrix * m_matrix * vec4(pos,1.0);
    coord = cd;
}
)";
    constexpr const static char * def_frag =
R"(#version 460 core


layout(binding = 0) uniform sampler2D tex;

out vec4 color;
in vec2 coord;


void main(){
    vec4 pick = texture2D(tex,coord);
    color = pick;
}
)";
    };
}

#endif // EME_INCL
