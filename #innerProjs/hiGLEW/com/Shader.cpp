#include "Shader.h"
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include "Utility.h"

using namespace me;
using namespace std;

Shader::Shader(bool x){
    memset(enabled,0,sizeof(bool) * ME_SHADER_TYPEC);
    vertex = fragment = geometry = 0;
    if(x)program = glCreateProgram();
    else program = 0;
}


void Shader::CreateProgram(){
    if(!program)program = glCreateProgram();
}

int Shader::LoadFromFile(const string&file,GLenum type){
    return LoadFromFile(file.c_str(),type,file.length());
}

int Shader::LoadFromFile(const char * file,GLenum type,size_t sz){
    if(!file)return ME_NO_DATA;

    ///Declarations
    size_t rsz = (sz == 0?strlen(file):sz);
    char * buf;
    int ret;

    if(rsz == 0)return ME_EMPTY_STRING;
    rsz = Utility::file_size(file);
    if(!rsz)return ME_BAD_IO;
    ///Create File
    FILE * f = fopen(file,"r");
    buf = new char[rsz];
    if(!buf)return ME_BAD_MEM;
    memset(buf,0,sizeof(char) * rsz);
    fread(buf,sizeof(char),rsz,f);
    fclose(f);
    ///Call LoadFromMemory to make shader
    ret = LoadFromMemory(buf,type,rsz);
    ///Free buffer
    delete buf;
    ///Return the value
    return ret;
}

int Shader::LoadFromMemory(const string&str,GLenum type){
    return LoadFromMemory(str.c_str(),type,str.length());
}

int Shader::LoadFromMemory(const char * str,GLenum type,size_t sz){
    if(!str)return ME_NO_DATA;

    size_t rsz = (sz == 0?strlen(str):sz);
    GLuint *target;

    if(rsz == 0)return ME_EMPTY_STRING;

    if(type == ME_SHADER_VERTEX){
        if(enabled[0])return ME_ALREADY_EXI;
        target = &vertex;
        enabled[0] = true;
    }else if(type == ME_SHADER_FRAGMENT){
        if(enabled[1])return ME_ALREADY_EXI;
        target = &fragment;
        enabled[1] = true;
    }else if(type == ME_SHADER_GEOMETRY){
        if(enabled[2])return ME_ALREADY_EXI;
        target = &geometry;
        enabled[2] = true;
    }else return ME_BAD_TYPE;
    *target = glCreateShader(type);
    glShaderSource(*target,1,&str,NULL);
    glCompileShader(*target);
    glAttachShader(program,*target);
    return ME_NO_ERROR;
}

void Shader::LinkShader(){
    glLinkProgram(program);
}

void Shader::bind(Shader*s){
    if(!s)glUseProgram(0);
    else glUseProgram(s->program);
}

void Shader::bind(){
    glUseProgram(program);
}

void Shader::Log(){
    Utility::PrintProgramLog(program);
    if(enabled[0])Utility::PrintShaderLog(vertex);
    if(enabled[1])Utility::PrintShaderLog(fragment);
    if(enabled[2])Utility::PrintShaderLog(geometry);
}


string Shader::GetLog(){
    string x = "";
    StoreLog(x);
    return x;
}

void Shader::StoreLog(string & v){
    Utility::GetProgramLog(program,v);
    if(enabled[0])Utility::GetShaderLog(vertex,v);
    if(enabled[1])Utility::GetShaderLog(fragment,v);
    if(enabled[2])Utility::GetShaderLog(geometry,v);
}

GLuint Shader::GetProgram(){return program;}

int Shader::GetFragmentShader(){
    if(enabled[1])return (int)fragment;
    return ME_NO_DATA;
}

int Shader::GetGeometryShader(){
    if(enabled[2])return (int)geometry;
    return ME_NO_DATA;
}

int Shader::GetVertexShader(){
    if(enabled[0])return (int)vertex;
    return ME_NO_DATA;
}

int Shader::LoadsFromFile(const char * vert,const char * frag,const char * geometry){
    return LoadFromFile(vert,ME_SHADER_VERTEX) |
    LoadFromFile(frag,ME_SHADER_FRAGMENT) |
    LoadFromFile(geometry,ME_SHADER_GEOMETRY);
}

int Shader::LoadsFromMem(const char * vert,const char * frag,const char * geometry){
    return LoadFromMemory(vert,ME_SHADER_VERTEX) |
    LoadFromMemory(frag,ME_SHADER_FRAGMENT) |
    LoadFromMemory(geometry,ME_SHADER_GEOMETRY);
}


int Shader::LoadLinkLogF(const char * vert,const char * frag,const char * geo){
    int ret = LoadsFromFile(vert,frag,geo);
    LinkShader();
    Log();
    return ret;
}

int Shader::LoadLinkLogM(const char * vert,const char * frag,const char * geo){
    int ret = LoadsFromMem(vert,frag,geo);
    LinkShader();
    Log();
    return ret;
}

GLfloat ShaderArg::operator=(GLfloat v){
    if(!ava)return v;
    glUniform1f(offset,v);
    return v;
}


GLdouble ShaderArg::UploadDouble(GLdouble v){
    if(!ava)return v;
    glUniform1d(offset,v);
    return v;
}

GLint ShaderArg::operator=(GLint v){
    if(!ava)return v;
    glUniform1i(offset,v);
    return v;
}

GLuint ShaderArg::operator=(GLuint v){
    if(!ava)return v;
    glUniform1ui(offset,v);
    return v;
}

GLfloat* ShaderArg::operator=(GLfloat* v){
    if(!ava)return v;
    glUniformMatrix4fv(offset,1,GL_FALSE,v);
    return v;
}

ShaderArg::ShaderArg(GLuint a,GLuint b){
    SetProgram(a);
    SetOffset(b);
}

GLuint ShaderArg::GetOffset(){return offset;}
GLuint ShaderArg::GetProgram(){return program;}
void ShaderArg::SetOffset(GLuint a){
    offset = a;
}
void ShaderArg::SetProgram(GLuint b){
    if(b){
        ava = true;
        program = b;
    }
}
bool ShaderArg::IsAvailable(){return ava;}


ShaderArg Shader::GetUniform(const char * s){
    return ShaderArg(program,glGetUniformLocation(program,s));
}

ShaderArg Shader::GetUniform(const string & s){
    return GetUniform(s.c_str());
}


ShaderArg Shader::operator[](const char * s){
    return ShaderArg(program,glGetUniformLocation(program,s));
}

ShaderArg Shader::operator[](string & s){
    return GetUniform(s.c_str());
}

glm::mat4* ShaderArg::operator=(glm::mat4 &v){
    (*this) = glm::value_ptr(v);
    return &v;
}
