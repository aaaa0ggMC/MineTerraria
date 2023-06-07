#include "Window.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <unistd.h>

using namespace me;

Window* Window::current = NULL;


void Window::Clear(bool clearColor,bool clearDepth){
    if(clearColor)glClear(GL_COLOR_BUFFER_BIT);
    if(clearDepth)glClear(GL_DEPTH_BUFFER_BIT);
}

Window::Window(int major,int minor){
    Utility::InitGLFW(major,minor);
    paint = NULL;
    win = NULL;
    curCam = NULL;
    flimit = frame_start = twait = 0;
    limitedF = false;
    press = NULL;
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
    firstTime = glfwGetTime();
    if(paint)paint(*this,glfwGetTime(),curCam);
    glfwSwapBuffers(win);
    glfwPollEvents();
    if(limitedF){
        while(glfwGetTime()+ME_FRAME_ADJUST_V < frame_start + twait){
            usleep((useconds_t)(twait)*1000 * 100);
        }
        frame_start += twait;
    }
    if(press)press(*this,glfwGetTime() - firstTime,curCam);
}


bool Window::KeyInputed(int key,int state){
    return glfwGetKey(win,key) == state;
}

void Window::SetPaintFunction(WPaintFn fn){
    paint = fn;
}

void Window::OnKeyPressEvent(OnKeyPress fn){
    press = fn;
}

void Window::SetFramerateLimit(unsigned int limit){
    if(!limit){
        limitedF = false;
        flimit = UINT_MAX;
        return;
    }
    limitedF = true;
    frame_start = glfwGetTime();
    flimit = limit;
    twait = 1.0 / flimit;
}

unsigned int Window::GetFramerateLimit(){
    return flimit;
}

GObject::GObject(float x,float y,float z){
    position.x = x;
    position.y = y;
    position.z = z;
    SetRotation(0,0,0);
}

void GObject::SetPosition(float x,float y,float z){
    position.x = x;
    position.y = y;
    position.z = z;
}

void GObject::SetPosition(glm::vec3 & v){
    position.x = v.x;
    position.y = v.y;
    position.z = v.z;
}

glm::vec3 GObject::GetPosition(){return position;}


void GObject::Move(float x,float y,float z){
    position.x += x;
    position.y += y;
    position.z += z;
}

void GObject::Move(glm::vec3 & v){
    position.x += v.x;
    position.y += v.y;
    position.z += v.z;
}

glm::mat4 * GObject::GetMat(){return &mat;}

void GObject::UpdateModelMat(){
    mat = glm::translate(glm::mat4(1.0),position);
    mat = mat * rmat;
}

void GObject::UpdateRotationMat(){
    rmat = glm::mat4(1.0);
    if(rotations.x != 0){
        rmat = glm::rotate(rmat,rotations.x,glm::vec3(1,0,0));
    }
    if(rotations.y != 0){
        rmat = glm::rotate(rmat,rotations.y,glm::vec3(0,1,0));
    }
    if(rotations.z != 0){
        rmat = glm::rotate(rmat,rotations.z,glm::vec3(0,0,1));
    }
}

///View Mat,not model
void Camera::UpdateModelMat(){
    mat = glm::translate(glm::mat4(1.0),glm::vec3(-position.x,-position.y,-position.z));
    mat = mat * rmat;
}

Camera::Camera(float x,float y,float z){
    SetPosition(x,y,z);
}

void Window::UseCamera(Camera & c){
    curCam = &c;
}

void Camera::BuildPerspec(float fieldOfView,float ratio,float nearPlane,float farPlane){
    perspec = glm::perspective(fieldOfView,ratio,nearPlane,farPlane);
}

void Camera::BuildPerspec(float fieldOfView,float width,float height,float nearPlane,float farPlane){
    BuildPerspec(fieldOfView,width/height,nearPlane,farPlane);
}

void Camera::BuildPerspec(float fieldOfView,void*w,float nearPlane,float farPlane){
    glm::vec2 sz = ((Window*)(w))->GetBufferSize();
    BuildPerspec(fieldOfView,sz.x / sz.y,nearPlane,farPlane);
}

glm::vec2 Window::GetWindowSize(){
    int w,h;
    glfwGetWindowSize(win,&w,&h);
    return glm::vec2(w,h);
}

void Window::Draw(GObject& o,GLuint triangles,GLuint in,GLuint bindingIndex){
    if(!in)return;
    if(o.vbo.GetVBO()){
        o.vbo.BindingTo(bindingIndex);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        if(in <= 1)glDrawArrays(GL_TRIANGLES, 0, triangles);
        else glDrawArraysInstanced(GL_TRIANGLES,0,triangles,in);
    }
}

glm::vec2 Window::GetBufferSize(){
    int w,h;
    glfwGetFramebufferSize(win,&w,&h);
    return glm::vec2(w,h);
}

void GObject::BuildMV(glm::mat4 * m){
    mvmat = *m * mat;
}

void GObject::BuildMV(GObject * v){
    BuildMV(&(v->mat));
}

VBO GObject::GetVBO(){return vbo;}

void GObject::SetRotation(float x,float y,float z){
    rotations = glm::vec3(x,y,z);
    UpdateRotationMat();
}

void GObject::BindVBO(VBO invbo){
    this->vbo = invbo;
}

void GObject::Rotate(float x,float y,float z){
    rotations += glm::vec3(x,y,z);
    ///re build rmat
    UpdateRotationMat();
}
