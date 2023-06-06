#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Utility.h"
#include "Shader.h"
#include <string>
#include <memory>

namespace me{
    using PShader = std::unique_ptr<Shader>;
    class Window{
    public:
        typedef void (*WPaintFn)(Window*,double currentTime);
        static Window * GetCurrent();
        Window(int major = 4,int minor = 3);
        int Create(unsigned int width,unsigned int height,const char * title,Window* share=NULL);
        int Create(unsigned int width,unsigned int height,std::string title,Window* share=NULL);
        GLFWwindow * GetGLFW();
        long GetSystemHandle();
        int CreateShader(std::unique_ptr<Shader> &shader);
        void MakeCurrent();
        static void MakeCurrent(Window *);
        void Destroy();
        static void SetSwapInterval(unsigned int = 1);
        bool ShouldClose();
        void Display();
        void SetPaintFunction(WPaintFn);
    private:
        static Window * current;
        GLFWwindow* win;
        WPaintFn paint;
    };
}


#endif // WINDOW_H_INCLUDED
