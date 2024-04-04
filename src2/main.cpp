#include <ME.h>
#include <CClock.h>
#include "-terrarian/Chunk.h"
#include "reses.h"
#define numVBOs 32
#define numVAOs 1


#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;
using namespace me;

Window window;
Shader shader(false);

Program game;

VBOs vbos;

Camera c(0,0,6);

GObject vcx(0,0,0),cube(0,-5,-5);

Velocity camSpeed(10);

VBOs vbo;
GLuint vao[numVAOs];

Texture test,test3d;

cck::Clock clk(false);

bool reloadTag = false;

unsigned int offset = 0;
const unsigned int maxium = 128;

void init();
void paint(Window& w,double currentTime,Camera*cam);
void input(Window& w,double elapseus,Camera * c);

MemFont testFont;

int main()
{
    Util::RegisterTerminal();
    window.Create(800,600,"UnlimitedLife",0);
    window.MakeCurrent();
    window.SetPaintFunction(paint);
    window.OnKeyPressEvent(input);
    window.UseCamera(c);


    testFont.LoadFromFile("res/fonts/rtest.ttf");


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

    clk.Start();
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

    //text.Activate(0);

    //glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D_ARRAY,test3d);
    test3d.Activate(0);

    glDisable(GL_CULL_FACE);

    shader.bind();
    vrp = c->vrp_matrix;

    glFrontFace(ME_CCW);
    m_mat = vcx.mat;
    w.Draw(vcx,6,maxium);

    ///
    glFrontFace(ME_CW);
    m_mat = cube.mat;
	w.Draw(cube,36);
	shader.unbind();

	///fps display
    static char buf[48];
    static unsigned int smfps = 0;
    static unsigned int all = 0;
    if(clk.Now().offset > 100){
        float elapse = clk.GetOffset();
        memset(buf,0,sizeof(char) * 48);
        sprintf(buf,"HiGLEW-% .2ffps | average %.2ffps | % .2fmspf",1000/elapse * smfps,1000 * all / clk.Now().all,elapse / smfps);
        SetWindowText((HWND)window.GetSystemHandle(),buf);
        smfps = 0;
        if(reloadTag){
            reloadTag = false;
            for(unsigned int ac = 0;ac < maxium;++ac){
                FT_GlyphSlot glyph = testFont.LoadChar(ac+offset+32);
                ///source depth should be 1,or the texture would'nt be updated
                test3d.UpdateGLTexture(glyph->bitmap.buffer,ac,glyph->bitmap.width,glyph->bitmap.rows,1,GL_RED,1);
                //glTexSubImage3D(GL_TEXTURE_2D_ARRAY,0,0,0,ac,glyph->bitmap.width,glyph->bitmap.rows,1,GL_RED,GL_UNSIGNED_BYTE,glyph->bitmap.buffer);
            }
            offset += maxium;
        }
    }
    smfps++;
    all++;
}

void init(){
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

    test.LoadFromFile("res/imgs/sb.png");
    test.UploadToOpenGL();

    test3d.Create2DTextureArray(64,64,maxium);

    for(unsigned int ac = 0;ac < maxium;++ac){
        FT_GlyphSlot glyph = testFont.LoadChar(ac+offset+32);
        test3d.UpdateGLTexture(glyph->bitmap.buffer,ac,glyph->bitmap.width,glyph->bitmap.rows,1,GL_RED,1,false);
        //glTexSubImage3D(GL_TEXTURE_2D_ARRAY,0,0,0,ac,glyph->bitmap.width,glyph->bitmap.rows,1,GL_RED,GL_UNSIGNED_BYTE,glyph->bitmap.buffer);
    }
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
        cx->Rotate(0,deg2rad(90 * elapseus),0);
    }else if(w.KeyInputed(GLFW_KEY_END) || w.KeyInputed(GLFW_KEY_RIGHT)){
        cx->Rotate(0,deg2rad(-90 * elapseus),0);
    }

    if(w.KeyInputed(GLFW_KEY_PAGE_UP) || w.KeyInputed(GLFW_KEY_UP)){
        cx->Rotate(deg2rad(60 * elapseus),0);
    }else if(w.KeyInputed(GLFW_KEY_PAGE_DOWN) || w.KeyInputed(GLFW_KEY_DOWN)){
        cx->Rotate(deg2rad(-60 * elapseus),0);
    }

    if(glfwGetKey(w.GetGLFW(),GLFW_KEY_M) == GLFW_PRESS){
        ///血的教训：由于input由glfw异步执行，因此这里无法使用OpenGL的大部分函数！
        ///Mistake!!:Due to the async-running of input executed by glfw,most of the functions of OpenGL are unavailable
        reloadTag = true;
    }
}
