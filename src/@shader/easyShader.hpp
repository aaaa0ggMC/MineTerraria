#ifndef EASYSHADER_HPP_INCLUDED
#define EASYSHADER_HPP_INCLUDED
#include "../helping.h"
#include <GL/gl.h>
#include <GL/glu.h>

using namespace sf;
using namespace std;


struct ShaderStatus{
    bool isAvailable;
    string GLVersion;//OpenGL�汾
    string vendor;//����
    string rendererToken;//��Ⱦ����ʶ
    string GLUVersion;//Glu�汾
    string GLSLVersion;//Glsl�汾
};

void ShaderInfo(ShaderStatus & ss){
    ss.isAvailable = Shader::isAvailable();
    ss.GLVersion = _p(glGetString(GL_VERSION));
    ss.vendor = _p(glGetString(GL_VENDOR));
    ss.rendererToken = _p(glGetString(GL_RENDERER));
    ss.GLUVersion = _p(gluGetString(GLU_VERSION));
}


#endif // EASYSHADER_HPP_INCLUDED
