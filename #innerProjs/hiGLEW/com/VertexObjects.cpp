#include "VertexObjects.h"
#include <iostream>

using namespace std;
using namespace me;

VBO::VBO(){
    vbo = 0;
}

VBO::VBO(GLuint v){
    vbo = v;
}

void VBO::Set(GLfloat d[],size_t sz){
    if(!vbo || !sz)return;
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sz,d,GL_STATIC_DRAW);
}

void VBO::Set(vector<GLfloat> v){
    (*this) = v;
}

void VBO::SetVBO(GLuint v){vbo = v;}

GLuint VBO::GetVBO(){return vbo;}

vector<GLfloat>* VBO::operator=(vector<GLfloat> & v){
    if(!vbo)return &v;
    size_t sz = v.size() * sizeof(GLfloat);
    GLfloat * buf = new GLfloat[sz];
    for(unsigned int i =0;i < sz;++i){
        buf[i] = v[i];
    }
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sz,buf,GL_STATIC_DRAW);
    delete buf;
    return &v;
}


void VBOs::AppendVBOs(unsigned int c){
    GLuint *gvbo = new GLuint[c];
    glGenBuffers(c,gvbo);
    for(unsigned int i = 0; i < c;++i){
        vbos.push_back(VBO(gvbo[i]));
    }
}

vector<GLuint> VBOs::GetGLVBOs(){
    vector<GLuint> uv;
    for(const VBO & va : vbos){
        uv.push_back(va.vbo);
    }
    return uv;
}

vector<VBO> VBOs::GetVBOs(){return vbos;}

VBO VBOs::operator[](unsigned int index){
    if(index >= vbos.size())return VBO(0);
    return vbos[index];
}

void VBO::bind(){
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
}


void VBO::AttributePointer(GLuint index,GLuint typesPerI,GLenum type,GLboolean normalized,GLsizei stride,const void * pointer){
    glVertexAttribPointer(index,typesPerI,type,normalized,stride,pointer);
}

void VBO::EnableArray(GLuint index){
    glEnableVertexAttribArray(index);
}


void VBO::BindingTo(GLuint index){
    this->bind();
    AttributePointer(index);
    EnableArray(index);
}
