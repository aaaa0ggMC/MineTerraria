#include "Utility.h"
#include <sys/stat.h>
#include <GLFW/glfw3.h>

using namespace me;
using namespace std;

bool me::Utility::initedGlew = false;
bool me::Utility::initedGLFW = false;

bool Utility::GetOpenGLError(std::string&appender,const char * sigStr){
    bool hasError = false;
    GLenum glErr;
    glErr = glGetError();
    while(glErr != GL_NO_ERROR){
        appender.append(sigStr);
        appender.append(to_string(glErr));
        appender.append("\n");
        hasError = true;
        glErr = glGetError();
    }
    return hasError;
}

void Utility::PrintOpenGLError(){
    string inner = "";
    if(GetOpenGLError(inner)){
        cout << inner << endl;
    }
}

size_t Utility::file_size(const char*filename){
    struct stat statbuf;
    int ret;
    ret = stat(filename,&statbuf);//����stat����
    if(ret != 0) return 0;//��ȡʧ�ܡ� 2023 6 5:�����иĶ�
    return statbuf.st_size;//�����ļ���С��
}

int Utility::InitGlew(){
    if(!initedGlew){
        int ret = glewInit();
        if(!ret)initedGlew = true;
        return ret;
    }
    return GLEW_OK;
}

void Utility::InitGLFW(int major,int minor){
    if(!initedGLFW){
        initedGLFW = true;
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,minor);
    }
}

bool Utility::GetShaderLog(GLuint shader,string&appender){
    int len = 0,chWritten = 0;
    glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&len);
    if(len <= 0)return false;
    char * data = new char[len];
    glGetShaderInfoLog(shader,len,&chWritten,data);
    appender.append(data);
    free(data);
    return true;
}

void Utility::PrintShaderLog(GLuint shader){
    string s = "";
    bool ret = GetShaderLog(shader,s);
    if(ret){
        cout << s << endl;
    }
}


bool Utility::GetProgramLog(GLuint shader,string&appender){
    int len = 0,chWritten = 0;
    glGetProgramiv(shader,GL_INFO_LOG_LENGTH,&len);
    if(len <= 0)return false;
    char * data = new char[len];
    glGetProgramInfoLog(shader,len,&chWritten,data);
    appender.append(data);
    free(data);
    return true;
}

void Utility::PrintProgramLog(GLuint shader){
    string s = "";
    bool ret = GetProgramLog(shader,s);
    if(ret){
        cout << s << endl;
    }
}
