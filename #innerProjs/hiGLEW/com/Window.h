///@Copyright aaaa0ggmc 2023
#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Utility.h"
#include "Shader.h"
#include "VertexObjects.h"
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

#define ME_FRAME_ADJUST_V 0.08

namespace me{
    using PShader = std::unique_ptr<Shader>;

    class GObject{
    public:
        GObject(float x = 0,float y = 0,float z = 0,bool enableMovement = false);

        void SetPosition(float x,float y,float z = 0);
        void SetPosition(glm::vec3& v);
        glm::vec3 GetPosition();

        void Move(float x,float y,float z = 0);
        void Move(glm::vec3 & v);
        void MoveDirectional(float left,float up,float forward = 0);

        glm::mat4* GetMat();
        virtual void UpdateModelMat();
        void UpdateRotationMat();

        void BuildMV(glm::mat4 * m);
        void BuildMV(GObject * g);

        void SetRotation(float x,float y,float z = 0);
        ///TODO：由于OpenGL体系中forward与相机初始角度不统一，导致相机z旋转有问题
        ///不过一般x,y旋转就够了
        void Rotate(float x,float y,float z = 0);
        void RotateDirectional(float left,float up,float forward = 0);
        void SetRotationD(float left,float up,float forward = 0);

        void BindVBO(VBO invbo);
        VBO GetVBO();

        void SetMovement(bool = true);

        bool movement;

        glm::vec3 rotations;
        glm::vec3 position;

        glm::mat4 mvmat;
        glm::mat4 mat;
        glm::mat4 rmat;

        glm::mat4 tempMat;

        glm::vec3 left;
        glm::vec3 forward;
        glm::vec3 up;

    private:
        friend class Window;
        VBO vbo;
    };

    class Camera : public GObject{
    public:
        glm::mat4 perspec;
        Camera(float x = 0,float y = 0,float z = 0,bool = true);
        void UpdateModelMat();
        void BuildPerspec(float fieldOfView,float ratio,float nearPlane,float farPlane);
        void BuildPerspec(float fieldOfView,float width,float height,float nearPlane,float farPlane);
        void BuildPerspec(float fieldOfView,void*w,float nearPlane,float farPlane);
        void BuildOrth(float left,float right,float bottom,float top);
    };

    class Window{
    public:
        typedef void (*WPaintFn)(Window&,double currentTime,Camera*cam);
        typedef void (*OnKeyPress)(Window&,double elapseus,Camera*cam);

        static Window * GetCurrent();
        static void MakeCurrent(Window *);
        //默认不开启垂直同步
        static void SetSwapInterval(unsigned int = 0);

        Window(int major = 4,int minor = 3);

        int Create(unsigned int width,unsigned int height,const char * title,Window* share=NULL);
        int Create(unsigned int width,unsigned int height,std::string title,Window* share=NULL);

        void Destroy();
        bool ShouldClose();
        void Display();
        void Clear(bool clearColor = true,bool clearDepth = true);

        GLFWwindow * GetGLFW();
        long GetSystemHandle();

        int CreateShader(std::unique_ptr<Shader> &shader);

        void MakeCurrent();
        unsigned int GetFramerateLimit();
        void SetFramerateLimit(unsigned int limit);

        void Draw(GObject&,GLuint targetC,GLuint instance = 1,GLuint bindingIndex = 0);
        void DrawModel(Model & model,GLuint instance = 1,GLuint bindingIndex = 0);

        void SetPaintFunction(WPaintFn);
        void OnKeyPressEvent(OnKeyPress);
        void UseCamera(Camera& cam);

        bool KeyInputed(int key,int state = GLFW_PRESS);

        void SetUIRange(float left,float top,float right,float bottom);

        glm::vec2 GetWindowSize();
        glm::vec2 GetBufferSize();

        Camera uiCam;
    private:
        static Window * current;
        GLFWwindow* win;
        WPaintFn paint;
        OnKeyPress press;
        unsigned int flimit;
        float twait,frame_start;
        bool limitedF;
        Camera * curCam;
        float firstTime;
    };
}


#endif // WINDOW_H_INCLUDED
