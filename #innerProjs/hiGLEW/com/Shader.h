///@Copyright aaaa0ggmc 2023
#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED
#include <GL/glew.h>
#include "Utility.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace me{
    using namespace std;

    class ShaderArg{
    public:
        ShaderArg(GLuint program = 0,GLuint offset = 0);
        GLuint GetOffset();
        GLuint GetProgram();
        void SetOffset(GLuint);
        void SetProgram(GLuint);
        bool IsAvailable();
        GLfloat operator=(GLfloat v);
        GLfloat* operator=(GLfloat* v);
        GLuint operator=(GLuint v);
        glm::mat4* operator=(glm::mat4& v);
        GLdouble UploadDouble(GLdouble v);
        GLint UploadInt(GLint v);
    private:
        bool ava;
        GLuint program;
        GLuint offset;
    };

    class Shader{
    public:
        void CreateProgram();
        ShaderArg GetUniform(const char * s);
        ShaderArg GetUniform(const string & s);
        ShaderArg operator[](const char * s);
        ShaderArg operator[](string & s);
        int LoadLinkLogF(const char * vert,const char * frag,const char * geo = NULL);
        int LoadLinkLogM(const char * vert,const char * frag,const char * geo = NULL);
        int LoadFromFile(const string&file,GLenum type);
        int LoadFromFile(const char * file,GLenum type,size_t sz = ME_DETECT_SIZE);
        int LoadFromMemory(const string&str,GLenum type);
        int LoadFromMemory(const char * str,GLenum type,size_t sz = ME_DETECT_SIZE);
        int LoadsFromFile(const char * vert,const char * frag,const char * geometry = NULL);
        int LoadsFromMem(const char * vert,const char * frag,const char * geometry = NULL);
        int GetVertexShader();
        int GetFragmentShader();
        int GetGeometryShader();
        GLuint GetProgram();
        void bind();
        void LinkShader();
        void Log();
        string GetLog();
        void StoreLog(string& appender);
        static void bind(Shader*);
        static void unbind();
        Shader(bool initProgram = true);
    private:
        friend class Window;
        GLuint program;
        GLuint vertex,fragment,geometry;
        bool enabled[ME_SHADER_TYPEC];
    };
}

#endif // SHADER_H_INCLUDED
