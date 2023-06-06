#include "Window.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

using namespace me;

Window* Window::current = NULL;

Window::Window(int major,int minor){
    Utility::InitGLFW(major,minor);
    paint = NULL;
    win = NULL;
}

int Window::Create(unsigned int width,unsigned int height,const char* title,Window*share){
    if(win)return ME_ALREADY_EXI;
    win = glfwCreateWindow(width,height,title,NULL,share?share->win:NULL);
    return win?ME_NO_ERROR:ME_BAD_MEM;
}


int Window::Create(unsigned int width,unsigned int height,std::string title,Window*share){
    return Create(width,height,title.c_str(),share);
}

GLFWwindow * Window::GetGLFW(){return win;}

long Window::GetSystemHandle(){
    return (long)glfwGetWin32Window(win);
}

int Window::CreateShader(std::unique_ptr<Shader>& s){
    if(current){
        s.reset(new Shader());
        return ME_NO_ERROR;
    }
    return ME_NO_DATA;
}

Window* Window::GetCurrent(){return current;}

void Window::MakeCurrent(){
    glfwMakeContextCurrent(this->win);
    current = this;
    Utility::InitGlew();
}

void Window::MakeCurrent(Window * v){
    if(v){
        glfwMakeContextCurrent(v->win);
    }else glfwMakeContextCurrent(NULL);
    current = v;
    Utility::InitGlew();
}

void Window::Destroy(){
    glfwDestroyWindow(win);
    this->win = NULL;
    MakeCurrent(NULL);
}

void Window::SetSwapInterval(unsigned int a){
    glfwSwapInterval(a);
}

bool Window::ShouldClose(){
    return glfwWindowShouldClose(win);
}

void Window::Display(){
    if(paint)paint(this,glfwGetTime());
    glfwSwapBuffers(win);
    glfwPollEvents();
}

void Window::SetPaintFunction(WPaintFn fn){
    paint = fn;
}
