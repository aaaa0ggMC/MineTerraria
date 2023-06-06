#define private public
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "com/Window.h"

#define numVAOs 1
#define numVBOs 1

GLuint vao[numVAOs];

using namespace std;
using namespace me;

unique_ptr<Shader> shader;

float inc = 0.01f;
float x = 0.0f;

int main(){
    Window win;
    win.Create(800,600,"Hello World!");
    win.MakeCurrent();
    win.SetSwapInterval();
    win.SetPaintFunction(
    [](Window * w,double time){
        static Shader & sh = *shader;
        glClear(GL_COLOR_BUFFER_BIT);
        sh.bind();
        sh["off"] = (x += inc);
        glDrawArrays(GL_TRIANGLES,0,3);
    }
    );

    win.CreateShader(shader);
    shader->LoadLinkLogF("res/0.vert","res/0.frag");

    glGenVertexArrays(numVAOs,vao);
    glBindVertexArray(vao[0]);

    while(!win.ShouldClose()){
        win.Display();
    }

    win.Destroy();
    glfwTerminate();
    return 0;
}
