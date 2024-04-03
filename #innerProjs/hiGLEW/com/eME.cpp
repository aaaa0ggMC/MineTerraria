#include "eME.h"

using namespace std;
using namespace me;

eApp2D::eApp2D(){
    Util::RegisterTerminal();
}

int eApp2D::init(unsigned int w,unsigned int h,string t,Window::OnKeyPress fn){
    int x = window.Create(w,h,t);
    if(x != ME_ENO_ERROR)return x;
    window.MakeCurrent();
    window.UseCamera(camera);
    window.OnKeyPressEvent(fn);

    glGenVertexArrays(numVAOs,vao);
    glBindVertexArray(vao[0]);

    vbo.AppendVBOs(numVBOs);

    float rect[] = {
    0,0,0,
    1,0,0,
    0,1,0,
    0,1,0,
    1,0,0,
    1,1,0
    };

    float vcoord[] = {
    0,1,
    1,1,
    0,0,
    0,0,
    1,1,
    1,0
    };

    vbo[0].Set(rect,sizeof(rect));

    vbo[1].tps = 2;
    vbo[1].Set(vcoord,sizeof(vcoord));

    templatePlane.BindVBO(vbo[0],vbo[1]);
    templatePlane.SetBindings(0,1);

    shader.CreateProgram();
    shader.LoadLinkLogM(def_vert,def_frag);

    return 0;
}

eApp2D::~eApp2D(){
    window.Destroy();
}

bool eApp2D::Running(){
    window.MakeCurrent();
    window.PollEvents();
    window.__Display_Pre();
    return window.ShouldClose();
}

void eApp2D::Display(){
    window.__Display_Aft();
}
