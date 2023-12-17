#include "com/ME.h"
#include <windows.h>
#include <CClock.h>

using namespace std;
using namespace me;
using namespace glm;

#define numVAOs 1
#define numVBOs 32

GLuint vao[numVAOs];
VBOs vbo;
Shader s(false),lightCube(false);
Camera cam(0,0,25,true);
GObject cube(0,4,0),pyramid(-5,2,0);
Model model(0,-2,0);
Window window;
Velocity camSpeed(10);
Texture txr;
GlobalLight light;
bool pause = false;

Program game;

void setupVertices(void) {
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

	float pyramidPos [54]= {
    -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // front face
	 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // right face
	 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // back face
	 -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // left face
	 -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // base – left front
	 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f // base – right back
	};

	float pc[36] = {
        0,0,1,0,.5,1, 0,0,1,0,.5,1,
        0,0,1,0,.5,1, 0,0,1,0,.5,1,
        0,0,1,1,0 ,1, 1,1,0,0,1 ,0
	};
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	vbo.AppendVBOs(numVBOs);
    vbo[0].Set(vertexPositions,sizeof(vertexPositions));
    vbo[1].Set(pyramidPos,sizeof(pyramidPos));
    vbo[2].Set(pc,sizeof(pc));
    vbo[2].tps = 2;
    cube.BindVBO(vbo[0]);
    cube.SetBindings();
    pyramid.BindVBO(vbo[1],vbo[2]);
    pyramid.SetBindings();
	cam.BuildPerspec(1.0472f, &window , 0.1f, 1000.0f);
	txr.LoadFromFile("res/test.png");
	txr.UploadToOpenGL();

	///不可以独自创建VBO！
	model.CreateVBOs(vbo[3],vbo[4]);
    model.LoadModelFromStlBin("res/xyz.stl");
//	model.LoadModelFromObj("res/test.obj");
	model.UploadToOpenGL();
	model.Scale(.1,.1,.1);
	model.SetRotation(-90,0,0);

	//cout << model.vertc / 3 << endl;

    game.PushObj({&cube,&pyramid,&model});

    ///Setup shader basics
	s["ambient.strength"] = .3f;
	s["ambient.color"].UploadVec4(1.0,1.0,1.0,1.0);
    s["material.color"].UploadVec4(1,1,1,1);
    s["material.shiness"] = 0.5f;
}

void display(Window& window, double currentTime,Camera* c) {
    ///Shader Default
    static ShaderArg m_matrix = s["m_matrix"];
    static ShaderArg blend = s["blend"];
    static ShaderArg observer = s["observer"];
    static ShaderArg lpos = s["l.position"];
    static ShaderArg lcol = s["l.color"];
    static ShaderArg lstr = s["l.strength"];
    static ShaderArg vrp = s["vrp_matrix"];
    ///Shader Light Cube
    static ShaderArg lstr_lc = lightCube["gcolor.strength"];
    static ShaderArg lcol_lc = lightCube["gcolor.color"];
    static ShaderArg vrp_lc = lightCube["vrp_matrix"];
    static ShaderArg m_matrix_lc = lightCube["m_matrix"];

    static cck::Clock clk;

    window.Clear();

	c->Update();
    game.Update();

    observer = c->position;
    lpos = cube.position;
	vrp = c->vrp_matrix;
	lstr = light.strength;
	lcol = light.color;

	vrp_lc = c->vrp_matrix;
	lstr_lc = light.strength;
	lcol_lc = light.color;

	txr.Activate(0);
	window.EnableCullFaces();
	window.EnableDepthTest();

	s.bind();
	///
    glFrontFace(ME_CCW);
	blend = 1.0f;
	m_matrix = pyramid.mat;
    window.Draw(pyramid,18);
    ///
    glDisable(GL_CULL_FACE);
	m_matrix = model.mat;
	window.DrawModel(model);
	glEnable(GL_CULL_FACE);

	lightCube.bind();
    ///
    glFrontFace(ME_CW);
    m_matrix_lc = cube.mat;
	window.Draw(cube,36);


    ///fps display
    static char buf[48];
    static unsigned int smfps = 0;
    if(clk.Now().offset > 100){
        float elapse = clk.GetOffset();
        memset(buf,0,sizeof(char) * 48);
        sprintf(buf,"HiGLEW-% .2ffps| % .2fmspf",1000/elapse * smfps,elapse / smfps);
        SetWindowText((HWND)window.GetSystemHandle(),buf);
        smfps = 0;
    }
    smfps++;
}

void input(Window& w,double elapseus,Camera * c){

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
    camSpeed.MoveDr(*c,elapseus);

    if(w.KeyInputed(GLFW_KEY_HOME) || w.KeyInputed(GLFW_KEY_LEFT)){
        cam.Rotate(0,deg2rad(90 * elapseus),0);
    }else if(w.KeyInputed(GLFW_KEY_END) || w.KeyInputed(GLFW_KEY_RIGHT)){
        cam.Rotate(0,deg2rad(-90 * elapseus),0);
    }

    if(w.KeyInputed(GLFW_KEY_PAGE_UP) || w.KeyInputed(GLFW_KEY_UP)){
        cam.Rotate(deg2rad(60 * elapseus),0);
    }else if(w.KeyInputed(GLFW_KEY_PAGE_DOWN) || w.KeyInputed(GLFW_KEY_DOWN)){
        cam.Rotate(deg2rad(-60 * elapseus),0);
    }

    if(w.KeyInputed(GLFW_KEY_I)){
        cube.Move(0,0,elapseus * 5);
    }else if(w.KeyInputed(GLFW_KEY_K)){
        cube.Move(0,0,-elapseus * 5);
    }
    if(w.KeyInputed(GLFW_KEY_J)){
        cube.Move(elapseus * 5,0,0);
    }else if(w.KeyInputed(GLFW_KEY_L)){
        cube.Move(-elapseus * 5,0,0);
    }

    if(w.KeyInputed(GLFW_KEY_T)){
        pause = !pause;
    }

    if(!pause){
        static float rtime = 0;

        rtime += elapseus;
        model.Rotate(0,elapseus,0);

        light.strength = 1.0;
        light.color.x = cos(rtime) + sin(0.3*rtime) + 0.01;
        light.color.y = sin(0.13*rtime) - cos(0.7 * rtime) + 0.03;
        light.color.z = cos(0.19*rtime) + sin(0.37 * rtime) + 0.05;
        light.color = glm::normalize(light.color);
    }
}

int main(void){
    Util::RegisterTerminal();
	window.Create(800, 600, "Chapter 4 - program 1" , NULL);
    window.MakeCurrent();
	window.SetPaintFunction(display);
	//window.SetFramerateLimit(60);
	window.UseCamera(cam);
	window.OnKeyPressEvent(input);
	//window.SetFramerateLimit(60);
    glfwSetWindowSizeCallback(window.GetGLFW(),
    [](GLFWwindow* w,int nw,int nh){
        float aspect = (float)nw / nh;
        glViewport(0,0,nw,nh);
        cam.BuildPerspec(1.0472f, aspect , 0.1f, 1000.0f);
    });

	s.CreateProgram();
	s.LoadLinkLogF("res/0.vert","res/0.frag");

	lightCube.CreateProgram();
	lightCube.LoadLinkLogF("res/lightSrc.vert","res/lightSrc.frag");

	setupVertices();

	while (!window.ShouldClose()) {
        glfwPollEvents();
		window.Display();
	}

	window.Destroy();
	return 0;
}
