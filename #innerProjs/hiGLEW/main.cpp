#define private public
#include "com/ME.h"

using namespace std;
using namespace me;
using namespace glm;

#define numVAOs 1
#define numVBOs 32

GLuint vao[numVAOs];
VBOs vbo;
Shader s(false),direct(false);
Camera cam(0,0,8,true);
GObject cube(0,-10,0),pyramid(-5,2,0);
Model model(0,-2,0);
Window window;
Velocity camSpeed(10);
Texture txr;

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
	model.CreateVBOs(vbo[3],vbo[4],vbo[5]);
	model.LoadModelFromFile("res/test.obj");
	model.UploadToOpenGL();

    game.PushObj({&cube,&pyramid,&model});
}

void display(Window& window, double currentTime,Camera* c) {
    static ShaderArg v_matrix = s["v_matrix"];
    static ShaderArg m_matrix = s["m_matrix"];
    static ShaderArg cr_matrix = s["cr_matrix"];
    static ShaderArg proj_matrix = s["proj_matrix"];
    static ShaderArg tf = s["tf"];
    static ShaderArg blend = s["blend"];

    window.Clear();
	s.bind();

	c->Update();
    game.Update();

	v_matrix = c->mat;
	cr_matrix = c->rmat;
	proj_matrix = cam.perspec;
	tf = (float)currentTime;

	txr.Activate(0);
	window.EnableCullFaces();
	window.EnableDepthTest();

	blend = 1.0f;
	m_matrix = pyramid.mat;
	window.SetFrontFace(ME_CCW);
	window.Draw(pyramid,18);

    window.SetFrontFace(ME_CW);
	blend = 0.0f;
	m_matrix = cube.mat;
	tf = (float)currentTime + 10.0f;
	window.Draw(cube,36);

    window.SetFrontFace(ME_CCW);
	m_matrix = model.mat;
	tf = (float)currentTime + 10.0f;
	window.DrawModel(model);
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

    if(w.KeyInputed(GLFW_KEY_LEFT)){
        cam.Rotate(0,deg2rad(45 * elapseus),0);
    }else if(w.KeyInputed(GLFW_KEY_RIGHT)){
        cam.Rotate(0,deg2rad(-45 * elapseus),0);
    }

    if(w.KeyInputed(GLFW_KEY_UP)){
        cam.Rotate(deg2rad(45 * elapseus),0);
    }else if(w.KeyInputed(GLFW_KEY_DOWN)){
        cam.Rotate(deg2rad(-45 * elapseus),0);
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
    glfwSetWindowSizeCallback(window.GetGLFW(),
    [](GLFWwindow* w,int nw,int nh){
        float aspect = (float)nw / nh;
        glViewport(0,0,nw,nh);
        cam.BuildPerspec(1.0472f, aspect , 0.1f, 1000.0f);
    });

	s.CreateProgram();
	s.LoadLinkLogF("res/0.vert","res/0.frag");

	setupVertices();

    Counter c(true);
	while (!window.ShouldClose()) {
		window.Display();
        c.Increase();
	}
	c.Stop();
	c.SimpOut();

	window.Destroy();
	return 0;
}
