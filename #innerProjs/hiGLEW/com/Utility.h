///@Copyright aaaa0ggmc 2023
#ifndef UTILITY_H_INCED
#define UTILITY_H_INCED
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <unordered_set>


#define ME_DETECT_SIZE 0

#define ME_NO_ERROR 0x00000000
#define ME_OPENGL_ERROR 0x00000001
#define ME_EMPTY_STRING 0x00000010
#define ME_BAD_IO 0x00000100
#define ME_BAD_MEM 0x00001000
#define ME_BAD_TYPE 0x00010000
#define ME_NO_DATA 0x00100000
#define ME_ALREADY_EXI 0x01000000

#define ME_SHADER_VERTEX GL_VERTEX_SHADER
#define ME_SHADER_FRAGMENT GL_FRAGMENT_SHADER
#define ME_SHADER_GEOMETRY GL_GEOMETRY_SHADER

#define ME_SHADER_TYPEC 3

#ifdef DEBUG
#define ME_SIV(msg,rsg) Utility::InvokeConsole(msg,true,__FUNCTION__,(long)this + rsg)
#define ME_SIVD(msg,rsg) Utility::InvokeConsole(msg,true,__FUNCTION__,rsg)
#else
#define ME_SIV(msg,rsg)
#define ME_SIVD(msg,rsg)
#endif // DEBUG


namespace me{
    using namespace std;

    class Counter{
    public:
        Counter(bool start = false);
        float GetCyclePerS();
        void ReStart();
        void Stop();
        void SimpOut();
        void Increase();
        double start,end;
        unsigned long cycles;
    };

    class Utility{
    public:
        static unordered_set<string> sessions;
        static bool initedGlew;
        static bool initedGLFW;
        static bool GetOpenGLError(string&appender,const char * sigStr = "Find OpenGL Error:");
        static void PrintOpenGLError();
        static size_t file_size(const char*filename);
        static int InitGlew();
        static void InitGLFW(int major = 4,int minor = 3);
        static bool GetShaderLog(GLuint shader,string&appender);
        static void PrintShaderLog(GLuint shader);
        static bool GetProgramLog(GLuint prog,string&appender);
        static void PrintProgramLog(GLuint prog);
        static void RegisterTerminal();
        static void OnTerminal();
        static void InvokeConsole(const char * s,bool onlyOnce = false,const char * sessionId = NULL,long = 0);
        Utility() = delete;
        ~Utility() = delete;
        void operator=(Utility&) = delete;
    };
}

#endif // UTILITY_H_INCED
