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
        GObject(float x = 0,float y = 0,float z = 0);
        void SetPosition(float x,float y,float z = 0);
        void SetPosition(glm::vec3& v);
        glm::vec3 GetPosition();
        void Move(float x,float y,float z = 0);
        void Move(glm::vec3 & v);
        glm::mat4* GetMat();
        virtual void UpdateModelMat();
        void UpdateRotationMat();
        void BuildMV(glm::mat4 * m);
        void BuildMV(GObject * g);
        void SetRotation(float x,float y,float z);
        void Rotate(float x,float y,float z);
        void BindVBO(VBO invbo);
        VBO GetVBO();
        glm::vec3 rotations;
        glm::mat4 mvmat;
        glm::vec3 position;
        glm::mat4 mat;
        glm::mat4 rmat;
    private:
        friend class Window;
        VBO vbo;
    };

    class Camera : public GObject{
    public:
        glm::mat4 perspec;
        Camera(float x = 0,float y = 0,float z = 0);
        void UpdateModelMat();
        void BuildPerspec(float fieldOfView,float ratio,float nearPlane,float farPlane);
        void BuildPerspec(float fieldOfView,float width,float height,float nearPlane,float farPlane);
        void BuildPerspec(float fieldOfView,void*w,float nearPlane,float farPlane);
    };

    class Window{
    public:
        typedef void (*WPaintFn)(Window&,double currentTime,Camera*cam);
        typedef void (*OnKeyPress)(Window&,double elapseus,Camera*cam);
        static Window * GetCurrent();
        Window(int major = 4,int minor = 3);
        int Create(unsigned int width,unsigned int height,const char * title,Window* share=NULL);
        int Create(unsigned int width,unsigned int height,std::string title,Window* share=NULL);
        GLFWwindow * GetGLFW();
        long GetSystemHandle();
        int CreateShader(std::unique_ptr<Shader> &shader);
        void MakeCurrent();
        static void MakeCurrent(Window *);
        unsigned int GetFramerateLimit();
        void Destroy();
        //Ĭ�ϲ�������ֱͬ��
        static void SetSwapInterval(unsigned int = 0);
        void SetFramerateLimit(unsigned int limit);
        bool ShouldClose();
        void Display();
        void Clear(bool clearColor = true,bool clearDepth = true);
        void SetPaintFunction(WPaintFn);
        void UseCamera(Camera& cam);
        void Draw(GObject&,GLuint triangles,GLuint instance = 1,GLuint bindingIndex = 0);
        void OnKeyPressEvent(OnKeyPress);
        bool KeyInputed(int key,int state = GLFW_PRESS);
        glm::vec2 GetWindowSize();
        glm::vec2 GetBufferSize();
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
