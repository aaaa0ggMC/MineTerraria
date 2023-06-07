#include <string>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include "com/Window.h"
#include "com/VertexObjects.h"

#define PI 3.1415926
#define rad2deg(r) (180*((r)/PI))
#define deg2rad(d) (((d)/180)*PI)

using namespace std;
using namespace me;
using namespace glm;

#define numVAOs 1
#define numVBOs 2

GLuint vao[numVAOs];
VBOs vbo;
Shader s(false);
Camera cam(0,0,8,true);
GObject cube(0,-2,0),pyramid(-5,2,0);
Window window;
float camSpeed = 10;

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
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	vbo.AppendVBOs(numVBOs+1);
    vbo[0].Set(vertexPositions,sizeof(vertexPositions));
    vbo[1].Set(pyramidPos,sizeof(pyramidPos));
    cube.BindVBO(vbo[0]);
    pyramid.BindVBO(vbo[1]);
	cam.BuildPerspec(1.0472f, &window , 0.1f, 1000.0f);
}

void display(Window& window, double currentTime,Camera* c) {
    window.Clear();
	s.bind();

	///ISSUE:相机旋转也要负转矩阵吗？？
	//cam.SetRotation(0,PI / 4,0);

	cam.UpdateModelMat();
	cube.UpdateModelMat();
	pyramid.UpdateModelMat();

	s["m_matrix"] = cube.mat;
	s["v_matrix"] = c->mat;
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
        c->MoveDirectional(0,0,camSpeed*elapseus);
    else if(w.KeyInputed(GLFW_KEY_W))
        c->MoveDirectional(0,0,-camSpeed*elapseus);

    if(w.KeyInputed(GLFW_KEY_Q)){
        cube.Rotate(0,0,deg2rad(0.5));
    }
}

int main(void){
    Utility::RegisterTerminal();

	window.Create(800, 600, "Chapter 4 - program 1" , NULL);
	window.MakeCurrent();
	window.SetPaintFunction(display);
	window.SetFramerateLimit(0);
	window.UseCamera(cam);
	window.OnKeyPressEvent(input);

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
