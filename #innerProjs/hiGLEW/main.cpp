#define private public
#include <string>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include "com/Texture.h"
#include "com/Window.h"
#include "com/VertexObjects.h"

#define PI 3.1415926
#define rad2deg(r) (180*((r)/PI))
#define deg2rad(d) (((d)/180)*PI)

using namespace std;
using namespace me;
using namespace glm;

#define numVAOs 1
#define numVBOs 4

GLuint vao[numVAOs];
VBOs vbo;
Shader s(false),direct(false);
Camera cam(0,0,8,true);
GObject cube(0,-2,0),pyramid(-5,2,0);
Window window;
float camSpeed = 10;
Texture txr;
Model test;

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
	glGenVertexArrays(1, vao);
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
}

void display(Window& window, double currentTime,Camera* c) {
    window.Clear();
	s.bind();

	///ISSUE:相机旋转也要负转矩阵吗？？
	//cam.SetRotation(0,PI / 4,0);

	cam.UpdateModelMat();
	cube.UpdateModelMat();
	pyramid.UpdateModelMat();

	s["tex"].UploadInt(0);
	txr.Activate(0);

	s["m_matrix"] = cube.mat;
	s["v_matrix"] = c->mat;
	s["cr_matrix"] = c->rmat;
	s["proj_matrix"] = cam.perspec;
	s["tf"] = (float)currentTime;


	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	window.Draw(cube,36);

	s["m_matrix"] = pyramid.mat;
	glFrontFace(GL_CCW);
	window.Draw(pyramid,18);
}

void input(Window& w,double elapseus,Camera * c){
    if(w.KeyInputed(GLFW_KEY_SPACE))
        c->MoveDirectional(0,camSpeed*elapseus,0);
    else if(w.KeyInputed(GLFW_KEY_LEFT_SHIFT))
        c->MoveDirectional(0,-camSpeed*elapseus,0);

    if(w.KeyInputed(GLFW_KEY_A))
        c->MoveDirectional(-camSpeed*elapseus,0,0);
    else if(w.KeyInputed(GLFW_KEY_D))
        c->MoveDirectional(camSpeed*elapseus,0,0);

    if(w.KeyInputed(GLFW_KEY_S))
        c->MoveDirectional(0,0,-camSpeed*elapseus);
    else if(w.KeyInputed(GLFW_KEY_W))
        c->MoveDirectional(0,0,camSpeed*elapseus);

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
    Utility::RegisterTerminal();

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
