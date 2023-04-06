#ifndef EASYSHADER_HPP_INCLUDED
#define EASYSHADER_HPP_INCLUDED
#include "../helping.h"
#include <GL/gl.h>
#include <GL/glu.h>

using namespace sf;
using namespace std;


struct ShaderStatus{
    bool isAvailable;
    string GLVersion;//OpenGL版本
    string vendor;//厂商
    string rendererToken;//渲染器标识
    string GLUVersion;//Glu版本
    string GLSLVersion;//Glsl版本
};

void ShaderInfo(ShaderStatus & ss){
    ss.isAvailable = Shader::isAvailable();
    ss.GLVersion = _p(glGetString(GL_VERSION));
    ss.vendor = _p(glGetString(GL_VENDOR));
    ss.rendererToken = _p(glGetString(GL_RENDERER));
    ss.GLUVersion = _p(gluGetString(GLU_VERSION));
}


#endif // EASYSHADER_HPP_INCLUDED
