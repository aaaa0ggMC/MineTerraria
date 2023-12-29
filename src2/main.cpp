#include <ME.h>
#include "reses.h"

using namespace std;
using namespace me;

#define  numVBOs 32
#define  numVAOs 1

Window window;
Shader shader(false);

Program game;

VBOs vbos;

Camera c(0,0,6);

GObject vcx(0,0,0),cube(0,-5,-5);

Velocity camSpeed(10);

VBOs vbo;
GLuint vao[numVAOs];

Texture test;

void init();
void paint(Window& w,double currentTime,Camera*cam);
void input(Window& w,double elapseus,Camera * c);

int main()
{
    Util::RegisterTerminal();
    window.Create(800,600,"UnlimitedLife",0);
    window.MakeCurrent();
    window.SetPaintFunction(paint);
    window.OnKeyPressEvent(input);
    window.UseCamera(c);


    ///Setup projection matrix
    ///长度给负值可以和透视投影方向齐平
    //c.BuildOrthA(4,4,-8,&window,-1,10000);
	c.BuildPerspec(1.0472f, &window , 0.1f, 1000.0f);
    glfwSetWindowSizeCallback(window.GetGLFW(),
    [](GLFWwindow* w,int nw,int nh){
        glViewport(0,0,nw,nh);
        //c.BuildOrthA(-4,-4,8,nw / (float)nh,-1,10000);
        c.BuildPerspec(1.0472f, &window , 0.1f, 1000.0f);
    });


    init();

    while(!window.ShouldClose()){
        window.MakeCurrent();
        window.PollEvents();
        window.Display();
    }
    window.Destroy();
    return 0;
}

void paint(Window& w,double currentTime,Camera*c){
    static ShaderArg m_mat = ShaderArg(shader.GetProgram(),1);
    static ShaderArg vrp = ShaderArg(shader.GetProgram(),2);

    window.Clear();

    c->Update();
    game.Update();


    test.Activate(0);

    glDisable(GL_CULL_FACE);

    shader.bind();
    vrp = c->vrp_matrix;

    m_mat = vcx.mat;
    w.Draw(vcx,6);

    ///
    glFrontFace(ME_CW);
    m_mat = cube.mat;
	w.Draw(cube,36);
}

void init(){
    glGenVertexArrays(numVAOs,vao);
    glBindVertexArray(vao[0]);


    vbo.AppendVBOs(numVBOs);

    float rect[] = {
    0,0,0,
    0,1,0,
    1,0,0,
    1,0,0,
    0,1,0,
    1,1,0
    };

    float vcoord[] = {
        0,0,
        0,.05,
        .01,0,
        .01,0,
        0,.01,
        .01,.01
    };

    float vertexPositions[108] = {
	 -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
	 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
	 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	 -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	 -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
	 -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
	 -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
	 -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
	};

    vbo[0].Set(rect,sizeof(rect));

    vbo[1].tps = 2;
    vbo[1].Set(vcoord,sizeof(vcoord));

    vbo[3].Set(vertexPositions,sizeof(vertexPositions));

    vcx.BindVBO(vbo[0],vbo[1]);
    vcx.SetBindings(0,1);
    cube.BindVBO(vbo[3]);
    vcx.SetBindings(0,1);

    shader.CreateProgram();
    shader.LoadLinkLogF(MAIN_VERT,MAIN_FRAG);
    game.PushObj({&vcx,&cube});

    test.LoadFromFile("res/imgs/bg.png");
    test.UploadToOpenGL();
}

void input(Window& w,double elapseus,Camera * cx){

    camSpeed.New();
    if(w.KeyInputed(GLFW_KEY_SPACE))
        camSpeed.Add(0,1,0);
    else if(w.KeyInputed(GLFW_KEY_LEFT_SHIFT))
        camSpeed.Add(0,-1,0);

    if(w.KeyInputed(GLFW_KEY_A))
        camSpeed.Add(-1,0,0);
    else if(w.KeyInputed(GLFW_KEY_D))
        camSpeed.Add(1,0,0);

    if(w.KeyInputed(GLFW_KEY_S))
        camSpeed.Add(0,0,-1);
    else if(w.KeyInputed(GLFW_KEY_W))
        camSpeed.Add(0,0,1);

    camSpeed.Form();
    camSpeed.MoveDr(*cx,elapseus);

    if(w.KeyInputed(GLFW_KEY_HOME) || w.KeyInputed(GLFW_KEY_LEFT)){
        c.Rotate(0,deg2rad(90 * elapseus),0);
    }else if(w.KeyInputed(GLFW_KEY_END) || w.KeyInputed(GLFW_KEY_RIGHT)){
        c.Rotate(0,deg2rad(-90 * elapseus),0);
    }

    if(w.KeyInputed(GLFW_KEY_PAGE_UP) || w.KeyInputed(GLFW_KEY_UP)){
        c.Rotate(deg2rad(60 * elapseus),0);
    }else if(w.KeyInputed(GLFW_KEY_PAGE_DOWN) || w.KeyInputed(GLFW_KEY_DOWN)){
        c.Rotate(deg2rad(-60 * elapseus),0);
    }
}
