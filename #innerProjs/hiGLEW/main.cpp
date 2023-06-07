#include <string>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include "com/Window.h"
#include "com/VertexObjects.h"

#define PI 3.1415926

using namespace std;
using namespace me;
using namespace glm;

#define numVAOs 1
#define numVBOs 2

GLuint vao[numVAOs];
VBOs vbo;
Shader s(false);
Camera cam(0,0,40);
GObject cube(0,-2,0);
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
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	vbo.AppendVBOs(numVBOs);
    vbo[0].Set(vertexPositions,sizeof(vertexPositions));
    cube.BindVBO(vbo[0]);
	cam.BuildPerspec(1.0472f, &window , 0.1f, 1000.0f);
}

void display(Window& window, double currentTime,Camera* c) {
    window.Clear();
	s.bind();

	///ISSUE:相机旋转也要负转矩阵吗？？
	//cam.SetRotation(0,PI / 4,0);

	cam.UpdateModelMat();
	cube.UpdateModelMat();

	s["m_matrix"] = cube.mat;
	s["v_matrix"] = c->mat;
	s["proj_matrix"] = cam.perspec;
	s["tf"] = (float)currentTime;

	window.Draw(cube,36,1);
}

void input(Window& w,double elapseus,Camera * c){
    if(w.KeyInputed(GLFW_KEY_SPACE))
        c->Move(0,camSpeed*elapseus,0);
    else if(w.KeyInputed(GLFW_KEY_LEFT_SHIFT))
        c->Move(0,-camSpeed*elapseus,0);

    if(w.KeyInputed(GLFW_KEY_A))
        c->Move(-camSpeed*elapseus,0,0);
    else if(w.KeyInputed(GLFW_KEY_D))
        c->Move(camSpeed*elapseus,0,0);

    if(w.KeyInputed(GLFW_KEY_S))
        c->Move(0,0,camSpeed*elapseus);
    else if(w.KeyInputed(GLFW_KEY_W))
        c->Move(0,0,-camSpeed*elapseus);
}

int main(void){
    Utility::RegisterTerminal();

	window.Create(800, 600, "Chapter 4 - program 1" , NULL);
	window.MakeCurrent();
	window.SetPaintFunction(display);
	window.SetFramerateLimit(60);
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
