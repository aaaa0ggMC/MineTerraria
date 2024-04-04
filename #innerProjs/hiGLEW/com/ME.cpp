///@Copyright aaaa0ggmc 2023
#include "ME.h"
#include <sys/stat.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <thread>
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <fstream>
#include <sstream>

#include <aaa_util.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_DDS
#define STBI_NO_PVR
#define STBI_NO_PKM
#define STBI_NO_QOI
#define STBI_NO_EXT
#include <stb_image.h>
#include <string.h>

using namespace me;
using namespace std;

///Util
unordered_set<string> Util::sessions;
bool me::Util::initedGlew = false;
bool me::Util::initedGLFW = false;
///MemFont Init
bool me::MemFont::inited = false;
FT_Library me::MemFont::library = NULL;

bool Util::GetOpenGLError(std::string&appender,const char * sigStr){
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

void Util::PrintOpenGLError(){
    string inner = "";
    if(GetOpenGLError(inner)){
        cout << inner << endl;
    }
}

size_t Util::file_size(const char*filename){
    struct stat statbuf;
    int ret;
    ret = stat(filename,&statbuf);//调用stat函数
    if(ret != 0) return 0;//获取失败。 2023 6 5:这里有改动
    return statbuf.st_size;//返回文件大小。
}

int Util::InitGlew(){
    if(!initedGlew){
        int ret = glewInit();
        if(!ret)initedGlew = true;
        return ret;
    }
    return GLEW_OK;
}

void Util::InitGLFW(int major,int minor){
    if(!initedGLFW){
        initedGLFW = true;
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,minor);
    }
}

bool Util::GetShaderLog(GLuint shader,string&appender){
    int len = 0,chWritten = 0;
    glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&len);
    if(len <= 0)return false;
    char * data = new char[len];
    glGetShaderInfoLog(shader,len,&chWritten,data);
    appender.append(data);
    free(data);
    return true;
}

void Util::PrintShaderLog(GLuint shader){
    string s = "";
    bool ret = GetShaderLog(shader,s);
    if(ret){
        cout << s << endl;
    }
}


bool Util::GetProgramLog(GLuint shader,string&appender){
    int len = 0,chWritten = 0;
    glGetProgramiv(shader,GL_INFO_LOG_LENGTH,&len);
    if(len <= 0)return false;
    char * data = new char[len];
    glGetProgramInfoLog(shader,len,&chWritten,data);
    appender.append(data);
    free(data);
    return true;
}

void Util::PrintProgramLog(GLuint shader){
    string s = "";
    bool ret = GetProgramLog(shader,s);
    if(ret){
        cout << s << endl;
    }
}

void Util::RegisterTerminal(){
    std::atexit(Util::OnTerminal);
}

void Util::OnTerminal(){
    #ifdef DEBUG
        cout << "Game Terminated" << endl;
    #endif
    glfwTerminate();
}

void Util::InvokeConsole(const char * s,bool onlyOnce,const char * sessionId,long sig){
    #ifdef DEBUG
        string sd = sessionId?sessionId:"";
        sd += " : ";
        sd += to_string(sig);
        auto it = sessions.find(sd);
        if(onlyOnce &&  it != sessions.end())return;
        if(it == sessions.end() && sessionId)sessions.insert(sd);
        cout << "Invoked[" << sd << "]:" << s << "\n";
    #endif // DEBUG
}

///Counter///
Counter::Counter(bool st){
    cycles = 0;
    end = 0;
    if(st)ReStart();
}

float Counter::GetCyclePerS(){
    double elapse = (end - start);
    return cycles / elapse;
}

void Counter::ReStart(){
    start = glfwGetTime();
}

void Counter::Stop(){
    end = glfwGetTime();
}

void Counter::Increase(){++cycles;}

void Counter::SimpOut(){
    cout << "FPS average:" << (end-start)*1000 << "ms " << GetCyclePerS() << "cycles/s\n";
}

///VBO
VBO::VBO(GLuint v,unsigned int vbot,unsigned int stride){
    vbo = v;
    drawMethod = GL_TRIANGLES;
    vbo_type = vbot;
    tps = 3;
    this->stride = stride;
}

void VBO::Set(vector<GLfloat> v){(*this) = v;}
void VBO::Set(GLfloat d[],size_t sz){
    if(!vbo || !sz)return;
    glBindBuffer(vbo_type,vbo);
    glBufferData(vbo_type,sz,d,GL_STATIC_DRAW);
}
vector<GLfloat>* VBO::operator=(vector<GLfloat> & v){
    if(!vbo)return &v;
    size_t sz = v.size() * sizeof(GLfloat);
    glBindBuffer(vbo_type,vbo);
    glBufferData(vbo_type,sz,&v[0],GL_STATIC_DRAW);
    return &v;
}

void VBO::Set(vector<GLint> v){(*this) = v;}
void VBO::Set(GLint d[],size_t sz){
    if(!vbo || !sz)return;
    glBindBuffer(vbo_type,vbo);
    glBufferData(vbo_type,sz,d,GL_STATIC_DRAW);
}
vector<GLint>* VBO::operator=(vector<GLint> & v){
    if(!vbo)return &v;
    size_t sz = v.size() * sizeof(GLint);
    glBindBuffer(vbo_type,vbo);
    glBufferData(vbo_type,sz,&v[0],GL_STATIC_DRAW);
    return &v;
}

void VBO::SetVBO(GLuint v){vbo = v;}
GLuint VBO::GetVBO(){return vbo;}



void VBO::bind(){
    glBindBuffer(vbo_type,vbo);
}


void VBO::AttributePointer(GLuint index,GLuint typesPerI,GLenum type,GLboolean normalized,GLsizei stride,const void * pointer){
    glVertexAttribPointer(index,typesPerI,type,normalized,stride,pointer);
}

void VBO::EnableArray(GLuint index){
    glEnableVertexAttribArray(index);
}

void VBO::unbind(){
    this->bind();
    glDisableVertexAttribArray(bidx);
}


void VBO::bind2(GLuint index){
    this->bind();
    if(vbo_type == ME_VBO_ELEMENT){
        ME_SIV("Element Buffer doesn't support being visited in shaders!",0);
        return;
    }
    AttributePointer(index,tps,GL_FLOAT,GL_FALSE,stride,0);
    bidx = index;
    EnableArray(index);
}

GLuint VBO::CreateNew(){
    ME_SIV("OpenGL requires to gen vertex arrays at a fixed point!",2);
    if(vbo){
        ME_SIV("already created vbo!",0);
        return vbo;
    }
    glGenVertexArrays(1,&vbo);
    if(vbo){
        return vbo;
    }
    ME_SIV("gen vbo failed",1);
    return 0;
}

///VBOs
VBOs::VBOs(){vbos.push_back(VBO(0));}
void VBOs::AppendVBOs(unsigned int c,unsigned int tp){
    GLuint *gvbo = new GLuint[c];
    glGenBuffers(c,gvbo);
    for(unsigned int i = 0; i < c;++i){
        //cout << gvbo[i] << endl;
        vbos.push_back(VBO(gvbo[i],tp));
    }
}
vector<GLuint> VBOs::GetGLVBOs(){
    vector<GLuint> uv;
    for(const VBO & va : vbos){
        uv.push_back(va.vbo);
    }
    uv.erase(uv.begin(),uv.begin()+1);
    return uv;
}

vector<VBO> VBOs::GetVBOs(){
    vector<VBO> ret = vbos;
    ret.erase(ret.begin(),ret.begin()+1);
    return ret;
}

VBO& VBOs::operator[](unsigned int index){
    if(index+1 >= vbos.size())return vbos[0];
    return vbos[index+1];
}

///ShaderArg
GLfloat ShaderArg::operator=(GLfloat v){
    if(!ava){
        ME_SIV("Uniform not available!",0);
        return v;
    }
    glProgramUniform1f(program,offset,v);
    return v;
}

glm::mat3& ShaderArg::operator=(glm::mat3& v){
    return UploadMat3(v);
}
glm::vec4& ShaderArg::operator=(glm::vec4& v){
    return UploadVec4(v);
}
glm::vec3& ShaderArg::operator=(glm::vec3& v){
    return UploadVec3(v);
}
glm::vec2& ShaderArg::operator=(glm::vec2& v){
    return UploadVec2(v);
}

void ShaderArg::UploadRaw(glm::mat4 m){
    this->operator=(glm::value_ptr(m));
}
void ShaderArg::UploadRaw(glm::mat3 m){
    this->UploadMat3(glm::value_ptr(m));
}
void ShaderArg::UploadRaw(glm::vec4 v){
    this->UploadVec4(glm::value_ptr(v));
}
void ShaderArg::UploadRaw(glm::vec3 v){
    this->UploadVec3(glm::value_ptr(v));
}
void ShaderArg::UploadRaw(glm::vec2 v){
    this->UploadVec2(glm::value_ptr(v));
}

GLdouble ShaderArg::UploadDouble(GLdouble v){
    if(!ava){
        ME_SIV("Uniform not available!",0);
        return v;
    }
    glProgramUniform1d(program,offset,v);
    return v;
}

GLint ShaderArg::UploadInt(GLint v){
    if(!ava){
        ME_SIV("Uniform not available!",0);
        return v;
    }
    glProgramUniform1i(program,offset,v);
    return v;
}

GLuint ShaderArg::operator=(GLuint v){
    if(!ava){
        ME_SIV("Uniform not available!",0);
        return v;
    }
    glProgramUniform1ui(program,offset,v);
    return v;
}

GLfloat* ShaderArg::operator=(GLfloat* v){
    if(!ava){
        ME_SIV("Uniform not available!",0);
        return v;
    }
    glProgramUniformMatrix4fv(program,offset,1,GL_FALSE,v);
    return v;
}

GLfloat* ShaderArg::UploadMat3(GLfloat* v){
    if(!ava){
        ME_SIV("Uniform not available!",0);
        return v;
    }
    glProgramUniformMatrix3fv(program,offset,1,GL_FALSE,v);
    return v;
}

GLfloat* ShaderArg::UploadVec3(GLfloat * v){
    if(!ava){
        ME_SIV("Uniform not available!",0);
        return v;
    }
    glProgramUniform3f(program,offset,v[0],v[1],v[2]);
    return v;
}

GLfloat* ShaderArg::UploadVec4(GLfloat * v){
    if(!ava){
        ME_SIV("Uniform not available!",0);
        return v;
    }
    glProgramUniform4f(program,offset,v[0],v[1],v[2],v[3]);
    return v;
}

GLfloat* ShaderArg::UploadVec2(GLfloat * v){
    if(!ava){
        ME_SIV("Uniform not available!",0);
        return v;
    }
    glProgramUniform2f(program,offset,v[0],v[1]);
    return v;
}

glm::vec4& ShaderArg::UploadVec4(glm::vec4& v){
    if(!ava){
        ME_SIV("Uniform not available!",0);
        return v;
    }
    glProgramUniform4f(program,offset,v.x,v.y,v.z,v.w);
    return v;
}

glm::vec3& ShaderArg::UploadVec3(glm::vec3& v){
    if(!ava){
        ME_SIV("Uniform not available!",0);
        return v;
    }
    glProgramUniform3f(program,offset,v.x,v.y,v.z);
    return v;
}

glm::vec2& ShaderArg::UploadVec2(glm::vec2& v){
    if(!ava){
        ME_SIV("Uniform not available!",0);
        return v;
    }
    glProgramUniform2f(program,offset,v.x,v.y);
    return v;
}

void ShaderArg::UploadVec4(float x,float y,float z,float w){
    if(!ava){
        ME_SIV("Uniform not available!",0);
        return;
    }
    glProgramUniform4f(program,offset,x,y,z,w);
}

void ShaderArg::UploadVec3(float x,float y,float z){
    if(!ava){
        ME_SIV("Uniform not available!",0);
        return;
    }
    glProgramUniform3f(program,offset,x,y,z);
}

void ShaderArg::UploadVec2(float x,float y){
    if(!ava){
        ME_SIV("Uniform not available!",0);
        return;
    }
    glProgramUniform2f(program,offset,x,y);
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
glm::mat4& ShaderArg::operator=(glm::mat4 &v){
    (*this) = glm::value_ptr(v);
    return v;
}
glm::mat3& ShaderArg::UploadMat3(glm::mat3 &v){
    UploadMat3((GLfloat*)glm::value_ptr(v));
    return v;
}

///Shader
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

///Shader
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
    if(!file){
        ME_SIV("Can't load a file whose path is " "[NULL]!",0);
        return ME_ENO_DATA;
    }
    ///Declarations
    size_t rsz = (sz == 0?strlen(file):sz);
    char * buf;
    int ret;

    if(rsz == 0){
        ME_SIV("Can't load a file whose path is " "[\\0]",1);
        return ME_EEMPTY_STRING;
    }
    rsz = Util::file_size(file);
    if(!rsz){
        ME_SIV("Can't get the file size!",2);
        return ME_EBAD_IO;
    }
    ///Create File
    FILE * f = fopen(file,"r");
    buf = new char[rsz];
    if(!buf)return ME_EBAD_MEM;
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
    if(!str){
        ME_SIV("string given NULL",0);
        return ME_ENO_DATA;
    }

    size_t rsz = (sz == 0?strlen(str):sz);
    GLuint *target;

    if(rsz == 0){
        ME_SIV("string given empty",1);
        return ME_EEMPTY_STRING;
    }
    if(type == ME_SHADER_VERTEX){
        if(enabled[0]){
            ME_SIV("vertex" " shader already exists!",2);
            return ME_EALREADY_EXI;
        }
        target = &vertex;
        enabled[0] = true;
    }else if(type == ME_SHADER_FRAGMENT){
        if(enabled[1]){
            ME_SIV("fragment" " shader already exists!",3);
            return ME_EALREADY_EXI;
        }
        target = &fragment;
        enabled[1] = true;
    }else if(type == ME_SHADER_GEOMETRY){
        if(enabled[2]){
            ME_SIV("geometry" " shader already exists!",4);
            return ME_EALREADY_EXI;
        }
        target = &geometry;
        enabled[2] = true;
    }else{
        ME_SIV("Bad type of shader.Only VS,FS & GS are supported now!",5);
        return ME_EBAD_TYPE;
    }
    *target = glCreateShader(type);
    glShaderSource(*target,1,&str,NULL);
    glCompileShader(*target);
    glAttachShader(program,*target);
    ///Free free shader data
    glDeleteShader(*target);
    return ME_ENO_ERROR;
}

void Shader::LinkShader(){
    glLinkProgram(program);
}

void Shader::bind(Shader*s){
    if(!s)glUseProgram(0);
    else glUseProgram(s->program);
}

void Shader::unbind(){
    glUseProgram(0);
}

void Shader::bind(){
    glUseProgram(program);
}

void Shader::Log(){
    Util::PrintProgramLog(program);
    if(enabled[0])Util::PrintShaderLog(vertex);
    if(enabled[1])Util::PrintShaderLog(fragment);
    if(enabled[2])Util::PrintShaderLog(geometry);
}


string Shader::GetLog(){
    string x = "";
    StoreLog(x);
    return x;
}

void Shader::StoreLog(string & v){
    Util::GetProgramLog(program,v);
    if(enabled[0])Util::GetShaderLog(vertex,v);
    if(enabled[1])Util::GetShaderLog(fragment,v);
    if(enabled[2])Util::GetShaderLog(geometry,v);
}

GLuint Shader::GetProgram(){return program;}

int Shader::GetFragmentShader(){
    if(enabled[1])return (int)fragment;
    ME_SIV("No fragment shader!",0);
    return ME_ENO_DATA;
}

int Shader::GetGeometryShader(){
    if(enabled[2])return (int)geometry;
    ME_SIV("No geometry shader!",1);
    return ME_ENO_DATA;
}

int Shader::GetVertexShader(){
    if(enabled[0])return (int)vertex;
    ME_SIV("No vertex shader!",0);
    return ME_ENO_DATA;
}

int Shader::LoadsFromFile(const char * vert,const char * frag,const char * geometry){
    return LoadFromFile(vert,ME_SHADER_VERTEX) |
    LoadFromFile(frag,ME_SHADER_FRAGMENT) |
    (geometry?LoadFromFile(geometry,ME_SHADER_GEOMETRY):ME_ENO_ERROR);
}

int Shader::LoadsFromMem(const char * vert,const char * frag,const char * geometry){
    return LoadFromMemory(vert,ME_SHADER_VERTEX) |
    LoadFromMemory(frag,ME_SHADER_FRAGMENT) |
    (geometry?LoadFromMemory(geometry,ME_SHADER_GEOMETRY):ME_ENO_ERROR);
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
///Texture
Texture::Texture(){
    data = NULL;
    handle = 0;
    type = GL_TEXTURE_2D;
    channels = width = height = depth = 0;
    deleteS  = false;
    dataAva = false;
    format = 0;
}

int Texture::LoadFromFile(const char * f){
    if(data){
        ME_SIV("data already created!",2);
        return ME_EALREADY_EXI;
    }
    if(!f){
        ME_SIV("given [NULL]",0);
        return ME_ENO_DATA;
    }
    size_t sz = strlen(f);
    if(!sz){
        ME_SIV("given \\0",1);
        return ME_EEMPTY_STRING;
    }
    sz = Util::file_size(f);
    if(!sz){
        ME_SIV("empty file!",2);
        return ME_EBAD_IO;
    }
    int w,h,ncs;
    unsigned char * d = stbi_load(f,&w,&h,&ncs,0);
    if(!d){
        string str = "Can't load file ";
        str += f;
        ME_SIV(str.c_str(),3);
        return ME_EBAD_IO;
    }
    data = d;
    channels = ncs;
    width = w;
    height = h;
    deleteS = true;
    dataAva = true;
    return ME_ENO_ERROR;
}

int Texture::LoadFromMem(unsigned char * d,size_t sz,bool copy){
    if(data){
        ME_SIV("data already created!",2);
        return ME_EALREADY_EXI;
    }
    if(!d){
        ME_SIV("given [NULL]",0);
        return ME_ENO_DATA;
    }
    if(copy && !sz){
        ME_SIV("If you want to copy the data,please give the size of the chunk.",1);
        return ME_EBAD_MEM;
    }
    if(copy){
        data = new unsigned char[sz];
        memcpy(data,d,sz);
    }else data = d;
    dataAva = true;
    return ME_ENO_ERROR;
}

int Texture::UploadToOpenGL(bool gmm,int rtp,unsigned int a,unsigned int b){
    GLuint casel = GL_RGB;

    if(!data){
        ME_SIV("You DID NOT load the data!",0);
        return ME_ENO_DATA;
    }
    if(type != GL_TEXTURE_2D){
        if(handle){
            ME_SIV("Incompatible type to load texture!Texture already exists!",0);
            return ME_EALREADY_EXI;
        }else type = GL_TEXTURE_2D;
    }

    if(!handle)glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, rtp);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, rtp);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, a);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, b);

    switch(channels){
    case 1:
        casel = GL_RED;
        break;
    case 2:
        casel = GL_LUMINANCE_ALPHA;
        break;
    case 4:
        casel = GL_RGBA;
        break;
    case 3:
        break;
    default:
        ME_SIV("Image file is empty",1);
        return ME_ENO_DATA;
    }
    format = casel;
    glTexImage2D(GL_TEXTURE_2D, 0, casel, width, height, 0, casel, GL_UNSIGNED_BYTE, data);
    if(gmm)glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D,0);

    return ME_ENO_ERROR;
}

GLuint Texture::GetHandle(){
    if(handle)return handle;
    ME_SIV("You didn't call UploadToOpenGL to upload the texture!",0);
    return 0;
}

void Texture::Activate(GLuint index){
    if(index >= 32){
        ME_SIV("only supports index below 32",0);
        return;
    }
    glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(type,handle);
	glBindTextureUnit(index,handle);
}

Texture::~Texture(){
    if(dataAva){
        if(deleteS){
            stbi_image_free(data);
        }else delete [] data;
    }
}

void Texture::Deactivate(GLuint i){
    glActiveTexture(GL_TEXTURE0 + i);
}

void Texture::FreeData(){
    if(deleteS){
        stbi_image_free(data);
    }else delete [] data;
    dataAva = false;
    data = NULL;
}

void Texture::FreeTexture(){
    glDeleteTextures(1,&handle);
    handle = 0;
    type = GL_TEXTURE_2D;
    format = 0;
}

int Texture::Create2DTextureArray(unsigned int w,unsigned int h,unsigned int d,GLuint format,GLuint sf,unsigned char* s){
    if(handle){
        ME_SIV("This texture had been gened.Use FreeTexture;btw u can call FreeData along with FreeTexture",0);
        return ME_EALREADY_EXI;
    }
    type = GL_TEXTURE_2D_ARRAY;
    this->format = format;
    width = w;
    height = h;
    depth = d;
    glGenTextures(1,&handle);
    glBindTexture(type,handle);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage3D(GL_TEXTURE_2D_ARRAY,0,format,w,h,d,0,sf,GL_UNSIGNED_BYTE,s);
    return ME_ENO_ERROR;
}

int Texture::ClearGLTexture(unsigned int start_off_contained,unsigned int end_off_contained,float r,float g,float b,float a){
    float fillD[4] = {r,g,b,a};
    if(!handle){
        return ME_ENO_DATA;
    }
    glBindTexture(type,handle);
    if(type == GL_TEXTURE_2D){
        glTexSubImage2D(type,0,0,0,width,height,format,GL_FLOAT,fillD);
    }else if(type == GL_TEXTURE_2D_ARRAY){
        glTexSubImage3D(type,0,0,0,start_off_contained,width,height,end_off_contained - start_off_contained + 1,GL_RED,GL_FLOAT,fillD);
        cout << glGetError() << endl;
    }
    return ME_ENO_ERROR;
}

int Texture::UpdateGLTexture(unsigned char * bytes,unsigned int depth_offset,unsigned int w,unsigned int h,unsigned int depth,GLuint format,unsigned int alignValue,bool clearTex){
    if(clearTex){
        ClearGLTexture(depth,depth);
    }
    if(type == GL_TEXTURE_2D){
        if(!bytes){
            bytes = data;
            w = width;
            h = height;
        }
        glBindTexture(type,handle);
        glPixelStorei(GL_UNPACK_ALIGNMENT,alignValue);
        glTexImage2D(type,0,format,w,h,0,format,GL_UNSIGNED_BYTE,bytes);
        glPixelStorei(GL_UNPACK_ALIGNMENT,4);
    }else if(type == GL_TEXTURE_2D_ARRAY){
        if(!bytes){
            ME_SIV("Empty data to load a texture array![this warning's once,so there maybe many warnings like this.]",0);
            return ME_ENO_DATA;
        }
        glBindTexture(type,handle);

        glPixelStorei(GL_UNPACK_ALIGNMENT,alignValue);
        glTexSubImage3D(type,0,0,0,depth_offset,w,h,depth,format,GL_UNSIGNED_BYTE,bytes);
        glPixelStorei(GL_UNPACK_ALIGNMENT,4);
    }
    return ME_ENO_ERROR;
}

///GObject
GObject::GObject(float x,float y,float z,bool m){
    position.x = x;
    position.y = y;
    position.z = z;
    scale = glm::vec3(1,1,1);
    movement = m;
    SetRotation(0,0,0);
    UpdateRotationMat();
    UpdateModelMat();
    vbo = coord = VBO(0);
    vbind = cbind = 0;
}

void GObject::Update(unsigned int){
    if(Match(ME_GROT)){
        UpdateRotationMat();
    }
    UpdateModelMat();
}

void GObject::SetPosition(float x,float y,float z){
    MarkDirty(ME_GMOD);
    position.x = x;
    position.y = y;
    position.z = z;
}


void GObject::SetBindings(unsigned int vb,unsigned int cb){
    vbind = vb;
    cbind = cb;
}

void GObject::SetPosition(glm::vec3 & v){
    MarkDirty(ME_GMOD);
    position.x = v.x;
    position.y = v.y;
    position.z = v.z;
}

glm::vec3 GObject::GetPosition(){return position;}
void GObject::Move(float x,float y,float z){
    MarkDirty(ME_GMOD);
    position.x += x;
    position.y += y;
    position.z += z;
}

void GObject::Move(glm::vec3 & v){
    MarkDirty(ME_GMOD);
    position.x += v.x;
    position.y += v.y;
    position.z += v.z;
}

glm::mat4 * GObject::GetMat(){return &mat;}

void GObject::UpdateModelMat(){
    mat = glm::scale(glm::mat4(1.0),scale);
    mat = glm::translate(mat,position);
    mat = mat * rmat;
}

void GObject::SetMovement(bool v){
    MarkDirty(ME_GROT);
    movement = v;
    Rotate(0,0,0);
}

void GObject::MoveDirectional(float l,float u,float f){
    if(!movement){
        ///处理错误，不需要很节省
        ME_SIV("Using MoveDirectional without setting the movement field!",0);
        return;
    }
    MarkDirty(ME_GROT);
    float x = l * left.x + u * up.x + f * forward.x;
    float y = l * left.y + u * up.y + f * forward.y;
    float z = l * left.z + u * up.z + f * forward.z;
    Move(x,y,z);
}
void GObject::BuildMV(glm::mat4 * m){
    mvmat = *m * mat;
}

void GObject::BuildMV(GObject * v){
    BuildMV(&(v->mat));
}

VBO GObject::GetVBO(){return vbo;}

void GObject::Rotate(float x,float y,float z){
    MarkDirty(ME_GROT);
    rotations += glm::vec3(x,y,z);
}

void GObject::SetRotation(float x,float y,float z){
    MarkDirty(ME_GROT);
    rotations = glm::vec3(x,y,z);
}

void GObject::UpdateRotationMat(){
    rmat = glm::rotate(glm::mat4(1.0),rotations.y,glm::vec3(0.0,1.0,0.0));
    rmat = glm::rotate(rmat,rotations.x,glm::vec3(1.0,0.0,0.0));
    rmat = glm::rotate(rmat,rotations.z,glm::vec3(0.0,0.0,1.0));
    left = rmat * glm::vec4(1,0,0,1);
    up = rmat * glm::vec4(0,1,0,1);
    forward = rmat * glm::vec4(0,0,-1,1);
}

void GObject::BindVBO(VBO invbo,VBO vx){
    this->vbo = invbo;
    this->coord = vx;
}

void GObject::Scale(float x,float y,float z){
    MarkDirty(ME_GMOD);
    scale.x *= x;
    scale.y *= y;
    scale.z *= z;
}

void GObject::SetScale(float x,float y,float z){
    MarkDirty(ME_GMOD);
    scale.x = x;
    scale.y = y;
    scale.z = z;
}


///Camera
void Camera::BuildOrth(float l,float r,float b,float t,float n,float f){
    perspec = glm::ortho(l,r,b,t,n,f);
}

//View Mat,not model
void Camera::UpdateModelMat(){
    mat = glm::translate(glm::mat4(1.0),glm::vec3(-position.x,-position.y,-position.z));
    if(updateVRP)vrp_matrix = perspec * glm::transpose(rmat) * mat;
//    mat = mat * rmat;转到另一个
}

Camera::Camera(float x,float y,float z,bool m,bool uvrp){
    SetPosition(x,y,z);
    movement = m;
    SetRotation(0,0,0);
    updateVRP = uvrp;
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

void Camera::BuildOrthA(float left,float top,float xlen,void*w,float znear,float zfar){
    glm::vec2 sz = ((Window*)(w))->GetBufferSize();
    BuildOrth(left,left + xlen,top + xlen * sz.y / sz.x,top,znear,zfar);
}
void Camera::BuildOrthA(float left,float top,float xlen,float aspectRatio,float znear,float zfar){
    BuildOrth(left,left + xlen,top + xlen / aspectRatio,top,znear,zfar);
}

///Window
Window* Window::current = NULL;
void Window::Clear(bool clearColor,bool clearDepth){
    if(clearColor)glClear(GL_COLOR_BUFFER_BIT);
    if(clearDepth)glClear(GL_DEPTH_BUFFER_BIT);
}

void Window::PollEvents(){
    glfwPollEvents();
}

void Window::EnableDepthTest(GLenum func){
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(func);
}

void Window::DisableDepthTest(){
	glDisable(GL_DEPTH_TEST);
}

void Window::EnableCullFaces(){
    glEnable(GL_CULL_FACE);
}
void Window::DisableCullFaces(){
    glDisable(GL_CULL_FACE);
}

void Window::SetFrontFace(unsigned int dr){
    glFrontFace(dr);
}

Window::Window(int major,int minor){
    Util::InitGLFW(major,minor);
    paint = NULL;
    win = NULL;
    curCam = NULL;
    flimit = frame_start = twait = 0;
    limitedF = false;
    press = NULL;
    interval = 120;
    isOpen = false;
}

void Window::SetCheckInputInterval(unsigned int i){
    interval = i;
}

void Window::SetUIRange(float l,float t,float r,float b){
    uiCam.BuildOrth(l,r,b,t);
}

int Window::Create(unsigned int width,unsigned int height,const char* title,Window*share){
    if(win){
        ME_SIV("The window is already created!",0);
        return ME_EALREADY_EXI;
    }
    win = glfwCreateWindow(width,height,title,NULL,share?share->win:NULL);
    isOpen = true;
    ///Create Input Interval
    intervalThread = std::thread(
    [](bool*isOpen,OnKeyPress*press,unsigned int*interval,Window*win){
        while(*isOpen){
            if(*press)(*press)(*win,1.0 / (*interval),win->curCam);
            std::this_thread::sleep_for(1000ms / (*interval));
        }
    },&isOpen,&press,&interval,this);
    return win?ME_ENO_ERROR:ME_EBAD_MEM;
}

void Window::SetDiscreteKeyListener(Window::DiscreteKeyListener l){
    keyListener = l;
    glfwSetKeyCallback(win,l);
}


int Window::Create(unsigned int width,unsigned int height,std::string title,Window*share){
    return Create(width,height,title.c_str(),share);
}

GLFWwindow * Window::GetGLFW(){return win;}

long Window::GetSystemHandle(){
    return (long)glfwGetWin32Window(win);
}

Window* Window::GetCurrent(){return current;}

void Window::MakeCurrent(){
    glfwMakeContextCurrent(this->win);
    current = this;
    Util::InitGlew();
}

void Window::MakeCurrent(Window * v){
    if(v){
        glfwMakeContextCurrent(v->win);
    }else glfwMakeContextCurrent(NULL);
    current = v;
    Util::InitGlew();
}

void Window::Destroy(){
    isOpen = false;
    glfwDestroyWindow(win);
    if(intervalThread.joinable())intervalThread.join();
    this->win = NULL;
    MakeCurrent(NULL);
}

void Window::SetSwapInterval(unsigned int a){
    glfwSwapInterval(a);
}

bool Window::ShouldClose(){
    return glfwWindowShouldClose(win);
}

void Window::__Display_Pre(){
    static bool warned = false;
    if(!curCam && !warned){
        warned = true;
        ME_SIV("The main camera is NULL!Game may crash!Use Window::UseCamera to activate!",0);
    }
}

void Window::__Display_Aft(){
    firstTime = glfwGetTime();
    glfwSwapBuffers(win);
    if(limitedF){
        while(glfwGetTime()+ME_FRAME_ADJUST_V < frame_start + twait){
            std::this_thread::sleep_for((twait)*100ms);
        }
        frame_start += twait;
    }
}

void Window::Display(){
    __Display_Pre();
    if(paint)paint(*this,glfwGetTime(),curCam);
    __Display_Aft();
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

void Window::UseCamera(Camera & c){
    curCam = &c;
}

glm::vec2 Window::GetWindowSize(){
    int w,h;
    glfwGetWindowSize(win,&w,&h);
    return glm::vec2(w,h);
}

void Window::Draw(GObject& o,GLuint verts,GLuint in){
    if(!in){
        ME_SIV("The count of instance is zero.",0);
        return;
    }
    if(o.vbo.GetVBO()){
        o.vbo.bind2(o.vbind);
        if(o.coord.GetVBO()){
            if(o.cbind != o.vbind)o.coord.bind2(o.cbind);
            else{
                ME_SIV("How can u just bind vertex buffer & coord buffer together?",1);
            }
        }
        if(in <= 1)glDrawArrays(o.vbo.drawMethod, 0, verts);
        else glDrawArraysInstanced(o.vbo.drawMethod,0,verts,in);
    }
}


void Window::DrawModel(Model & model,GLuint in,GLuint vert,GLuint norm){
    if(!in){
        ME_SIV("The count of instance is zero.",0);
        return;
    }
    model.SetBindings(vert,norm);
    if(in <= 1)glDrawArrays(model.vbo.drawMethod,0,model.vertc);
    else glDrawArraysInstanced(model.vbo.drawMethod,0,model.vertc,in);
    model.Unbind();
}

glm::vec2 Window::GetBufferSize(){
    int w,h;
    glfwGetFramebufferSize(win,&w,&h);
    return glm::vec2(w,h);
}

///Model
int Model::LoadModelFromObj(const char * fname){
    unique_ptr<ObjLoader> obj;
    obj.reset(new ObjLoader(vfloats,nfloats,tfloats,vertc));
    return obj->LoadFromObj(fname)?ME_ENO_ERROR:ME_EBAD_IO;
}

int Model::LoadModelFromStlBin(const char * fname){
    unique_ptr<ObjLoader> obj;
    obj.reset(new ObjLoader(vfloats,nfloats,tfloats,vertc));
    return obj->LoadFromStlBin(fname)?ME_ENO_ERROR:ME_EBAD_IO;
}

void Model::CreateVBOs(VBO&vvbo,VBO& nvbo){
    vbo = vvbo;
//    ivbo = vbo1;
//    ivbo.vbo_type = ME_VBO_ELEMENT;
    this->nvbo = nvbo;
}

void Model::Unbind(){
    vbo.unbind();
//    ivbo.unbind();
    nvbo.unbind();
}

void Model::UploadToOpenGL(){
    if(!vbo.GetVBO()){
        ME_SIV("Some vbos are unavailable!",0);
        return;
    }
    vbo.Set(&(vfloats[0]),vfloats.size() * sizeof(float));
//    ivbo.Set(&(indices[0]),indices.size() * sizeof(float));
    if(nfloats.size() > 0){
        hasNormal = true;
        nvbo.Set(&(nfloats[0]),nfloats.size()*sizeof(float));
    }
}

void Model::SetBindings(GLuint v,GLuint v1){
    vbo.bind2(v);
//    ivbo.bind();
    if(hasNormal){
        nvbo.bind2(v1);
    }
}

void Model::SetData(vector<float>* vc,vector<float>* texc,vector<float>* norm){
    if(!vc & !texc & !norm){ME_SIV("empty call",0);return;}
    if(vc)vfloats = *vc;
    if(texc)tfloats = *texc;
    if(norm)nfloats = *norm;
}

Model::Model(float x,float y,float z){
    GObject(x,y,z);
    hasNormal = false;
}

///GLSupport
bool GLSupport::Check(GLSupport::GLType tp){
    switch(tp){
    case AnisotropicFilter:
        return glewIsSupported(EXT_AF);
    }
    return false;
}

bool GLSupport::Enable(GLSupport::GLType tp,float v){
    if(!Check(tp)){
        Util::InvokeConsole("given option isn't supported!",false);
        return false;
    }
    switch(tp){
        case AnisotropicFilter:{
            GLfloat sett;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&sett);
            if( v>=0 && v<sett)sett = v;
            glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,sett);
            return true;
        }
    }
    return false;
}

///Velocity
Velocity::Velocity(float v){
    vv = glm::vec3(0,0,0);
    SetVelocity(v);
}

void Velocity::New(){
    vv.x = vv.y = vv.z = 0;
}

void Velocity::Add(int x,int y,int z){
    vv.x += x;
    vv.y += y;
    vv.z += z;
}

void Velocity::Form(){
    float len = glm::length(vv);
    if(len)vv = v / len * vv;
}

void Velocity::MoveDr(GObject & g,float et){
    g.MoveDirectional(vv.x * et,vv.y * et,vv.z * et);
}

void Velocity::Move(GObject & g,float et){
    g.Move(vv.x * et,vv.y * et,vv.z * et);
}

void Velocity::SetVelocity(float cc){v = cc;}

///TODO ME_SIV!!!! When return false
///ObjLoader
bool ObjLoader::LoadFromObj(const char * obj_path){
   char token;
   string rest;
   glm::vec3 v;
   string mtl = "";
   vertc = 0;
   ///Load Obj
   if(!obj_path)return false;
   stringstream fobj;
   ifstream ffobj(obj_path);
   if(ffobj.bad())return false;
   //Read all the data
   {
     ffobj.seekg(0,ios::end);
     int sz = ffobj.tellg();
     ffobj.clear();
     ffobj.seekg(0,ios::beg);
     char * buf = new char[sz+1];
     memset(buf,0,sizeof(char) * (sz+1));
     ffobj.read(buf,sz);
     fobj.str(buf);
     delete buf;
     ffobj.close();
   }

   while(!fobj.eof()){
    fobj >> token;
    switch(token){
    //vertices
    case 'v':
        v.x = v.y = v.z = 0;
        fobj.get(token);
        //fobj >> token; 用符号>>会跳过空白 use operator ">>" would skip spaces
        if(token == ' '){
            fobj >> v.x >> v.y >> v.z;
//            cout << "V:" << v.x << " " << v.y << " " << v.z << endl;
            vertices.push_back(v);
        }else if(token == 'n'){
            fobj >> v.x >> v.y >> v.z;
//            cout << "N:" << v.x << " " << v.y << " " << v.z << endl;
            normals.push_back(v);
        }else if(token == 't'){
            fobj >> v.x >> v.y;
//            cout << "T:" << v.x << " " << v.y << endl;
            tcoords.push_back(glm::vec2(v.x,v.y));
        }
        break;
    //faces
    case 'f':{
        ///注意:face的所有起始索引为1而非0，注意对齐
        int index = 0;
        vertc += 3;
        for(int xx = 0;xx < 3;++xx){
            ///原来顺序是vtn,vertex/texcoord/normal 气死我了！
            fobj >> index;
            if(index != 0){
//                vindices.push_back(index-1);
                vfloats.push_back(vertices[index-1].x);
                vfloats.push_back(vertices[index-1].y);
                vfloats.push_back(vertices[index-1].z);
            }
            fobj.get(token);
            if(token != '/')continue;

            fobj >> index;
            ///缺点：f标签必须在v,vt,vn标签之后
            if(index != 0){
//                tindices.push_back(index-1);
                vtexc.push_back(tcoords[index-1].x);
                vtexc.push_back(tcoords[index-1].y);
            }
            fobj.get(token);
            if(token != '/')continue;

            fobj >> index;
            if(index != 0){
//                nindices.push_back(index-1);
                vnormals.push_back(normals[index-1].x);
                vnormals.push_back(normals[index-1].y);
                vnormals.push_back(normals[index-1].z);
            }
            //cout << "FACE:" << vindices[vindices.size()-1] << " "
            //<< nindices[nindices.size()-1] << " "
            //<< tindices[tindices.size()-1] << " " << endl;
        }
        break;
    }
//    case 'u':
//        fobj >> rest;
//        if(rest.compare("semtl"))break;
//        ///TODO:USE MTL
//        break;
//    case 'm':
//        fobj >> rest;
//        if(rest.compare("tllib"))break;
//        ///TODO:MTL LIB
//        break;
    default:
        getline(fobj,rest);
        //cout << "Skipped:" << token << rest << endl;
        break;
    }
   }
   ///TODO:READ MTL
   return true;
}

bool ObjLoader::LoadFromStlBin(const char * fp){
    if(!fp)return false;
    FILE * file = fopen(fp,"rb");
    if(!file)return false;
    ///Skip the first eighty chars
    fseek(file,80,SEEK_SET);
    vertc = 0;
    fread(&vertc,4,1,file);
    float v;
    for(unsigned int i = 0;i < vertc;++i){
        ///Normal
        v = 0;
        for(unsigned int ix = 0;ix < 3;++ix){
            fread(&v,4,1,file);
            vnormals.push_back(v);
        }
        ///repeat for 2 times
        for(unsigned int ix = 0;ix < 3*2;++ix){
            vnormals.push_back(vnormals[vnormals.size()-3]);
        }
        ///vertices
        for(unsigned int iy = 0;iy < 3;++iy){
            v = 0;
            for(unsigned int ix = 0;ix < 3;++ix){
                fread(&v,4,1,file);
                vfloats.push_back(v);
            }
        }
        fread(&v,2,1,file);
    }
    //給的是面的數量
    vertc *= 3;
    fclose(file);
    return true;
}

ObjLoader::ObjLoader(vector<float>&vf,vector<float>& nm,vector<float>& tc,unsigned int& fc):
    vfloats(vf),vnormals(nm),vtexc(tc),vertc(fc){}

///Changer
Changer::Changer(){
    dirty = 0;
}

void Changer::MarkDirty(int m){
    dirty |= m;
}

int Changer::GetDirty(){
    return dirty;
}

bool Changer::Match(int m){
    return dirty & m;
}

///Program
void Program::PushObj(vector<GObject*> l){
    for(auto a : l){
        objs.push_back(a);
    }
}

void Program::Update(){
    for(GObject*o : objs){
        o->Update();
    }
}

///MemFont
MemFont::MemFont(unsigned int font_size,unsigned int def_atrribute,unsigned int bs){
    attribute = def_atrribute;
    this->font_sizexy = font_size;

    face = NULL;
    bold_strengthxy = bs;

    ///Init Library
    if(!inited){
        inited = true;
        FT_Error err = FT_Init_FreeType(&library);
        if(err){
            ME_SIV("Freetype has failed to be inited!",0);
            inited = false;
        }else{
            atexit([]{
                FT_Done_FreeType(library);
            });
        }
    }
}

MemFont::~MemFont(){
    if(face){
        FT_Outline_Done(library,&outline);
        FT_Done_Face(face);
    }
}

int MemFont::LoadFromFile(const char * fp,unsigned int face_index){
    if(!inited){
        ME_SIV("Freetype hasn't been inited!",0);
        return ME_EFONT_BAD_FREETYPE;
    }
    if(!fp){
        ME_SIV("empty file path",0);
        return ME_ENO_DATA;
    }
    ///release old face
    if(face)FT_Done_Face(face);
    face = NULL;
    FT_Error err = FT_New_Face(library,fp,face_index,&face);
    if(err)return err;
    FT_Outline_New(library,ME_FONT_OUTLINE_NUM_POINTS,ME_FONT_OUTLINE_NUM_CONTOURS,&outline);

    SetFormat(ME_FONT_ATTR_PARENT);
    FT_Select_Charmap(face,FT_ENCODING_GB2312);
    return ME_ENO_ERROR;
}

int MemFont::LoadFromMem(unsigned char * buffer,unsigned long size,unsigned int face_index){
    if(!inited){
        ME_SIV("Freetype hasn't been inited!",0);
        return ME_EFONT_BAD_FREETYPE;
    }
    if(!buffer){
        ME_SIV("empty data",0);
        return ME_ENO_DATA;
    }
    if(face)FT_Done_Face(face);
    face = NULL;
    FT_Error err = FT_New_Memory_Face(library,buffer,size,face_index,&face);
    if(err)return err;
    FT_Outline_New(library,ME_FONT_OUTLINE_NUM_POINTS,ME_FONT_OUTLINE_NUM_CONTOURS,&outline);

    SetFormat(ME_FONT_ATTR_PARENT);

    FT_Select_Charmap(face,FT_ENCODING_GB2312);
    return ME_ENO_ERROR;
}

void MemFont::SetFormat(unsigned int att){
    if(att | ME_FONT_ATTR_PARENT)att = attribute;
    FT_Set_Pixel_Sizes(face,font_sizexy & ME_FONTSIZE_MASK,font_sizexy >> ME_FONTSIZE_OFFSET);
    if(att | ME_FONT_ATTR_BOLD)FT_Outline_EmboldenXY(&outline,bold_strengthxy & ME_BOLD_MASK,bold_strengthxy >> ME_BOLD_OFFSET);
}

void MemFont::SetDefSize(unsigned short font_sizex,unsigned short font_sizey){
    this->font_sizexy = ME_FONTSIZE(font_sizex,font_sizey);
}

void MemFont::SetDefAttribute(unsigned int attribute){
    ///TODO:italic is processed in shader rather than here
    this->attribute = attribute;
}

void MemFont::SetDefBoldStrength(unsigned short bx,unsigned short by){
    bold_strengthxy = ME_BOLD(bx,by);
}

FT_GlyphSlot MemFont::LoadChar(FT_ULong charcode_gbk,unsigned int attri,bool render){
    if(!face){
        ME_SIV("Empty face",0);
        return NULL;
    }
    FT_UInt index = FT_Get_Char_Index(face,charcode_gbk);
    FT_Load_Glyph(face,index,FT_LOAD_DEFAULT);
    SetFormat(attri);
    if(attri & ME_FONT_ATTR_PARENT)attri = attribute;
    if(attri & ME_FONT_ATTR_BOLD){
        FT_Outline_Copy(&outline,&(face->glyph->outline));
    }
    if(render){
        FT_Render_Glyph(face->glyph,FT_RENDER_MODE_NORMAL);
    }
    SetFormat(attribute);
    return face->glyph;
}

FT_GlyphSlot MemFont::LoadCharEx(FT_ULong charcode_gbk,unsigned int font_sizexy,unsigned int attri,bool render){
    if(!face){
        ME_SIV("Empty face",0);
        return NULL;
    }
    FT_UInt index = FT_Get_Char_Index(face,charcode_gbk);
    FT_Load_Glyph(face,index,FT_LOAD_DEFAULT);
    unsigned int oldxy = this->font_sizexy;
    this->font_sizexy = font_sizexy;
    SetFormat(attri);
    if(attri & ME_FONT_ATTR_PARENT)attri = attribute;
    if(attri & ME_FONT_ATTR_BOLD){
        FT_Outline_Copy(&outline,&(face->glyph->outline));
    }
    if(render){
        FT_Render_Glyph(face->glyph,FT_RENDER_MODE_NORMAL);
    }
    this->font_sizexy = oldxy;
    SetFormat(attribute);
    return face->glyph;
}

void MemFont::GenChar(FT_Glyph & target,FT_ULong charcode_gbk,unsigned int attri,bool render){
    FT_GlyphSlot glyph = LoadChar(charcode_gbk,attri,render);
    if(glyph)FT_Get_Glyph(glyph,&target);
}

///GlFont
GlFont::GlFont(unsigned int width,unsigned int height,
               unsigned int depth,unsigned int font_sizexy,
               unsigned int def_atrribute,unsigned int bold_strengthxy):memfont(font_sizexy,def_atrribute,bold_strengthxy){
    this->width = width;
    this->height = height;
    this->depth = depth;
}

inline void GlFont::SetSize(unsigned short x,unsigned short y){memfont.SetDefSize(x,y);};
inline void GlFont::SetBoldStrength(unsigned short bx,unsigned short by){memfont.SetDefBoldStrength(bx,by);}
inline void GlFont::SetAttribute(unsigned int att){memfont.SetDefAttribute(att);}
