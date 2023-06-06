#include <string>
#include <iostream>
#include "com/Window.h"
#include "com/VertexObjects.h"

using namespace std;
using namespace me;
using namespace glm;

#define numVAOs 1
#define numVBOs 2

GLuint vao[numVAOs];
VBOs vbo;
Shader s(false);
Camera cam(0,0,8);
GObject cube(0,-2,0);

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
}

void display(Window& window, double currentTime,Camera* c) {
    window.Clear();
	s.bind();

	cube.SetPosition(sin(currentTime),-2,cos(currentTime));
    cube.Rotate(0.1,-0.1,0.1);

	cam.UpdateMat();
	cube.UpdateMat();

	cube.BuildMV(&cam);

	s["mv_matrix"] = cube.mvmat;
	s["proj_matrix"] = cam.perspec;

	vbo[0].BindingTo(0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main(void){
	Window window;
	window.Create(800, 600, "Chapter 4 - program 1" , NULL);
	window.MakeCurrent();
	window.SetPaintFunction(display);
	window.SetFramerateLimit(60);
	s.CreateProgram();
	s.LoadLinkLogF("res/0.vert","res/0.frag");
	setupVertices();
	cam.BuildPerspec(1.0472f, &window , 0.1f, 1000.0f);

	while (!window.ShouldClose()) {
		window.Display();
	}

	window.Destroy();
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
