#ifndef ME_H_INCLUDED
#define ME_H_INCLUDED
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <iostream>
#include <unordered_set>
#include <vector>

///macros
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

#define ME_VBO_ELEMENT GL_ELEMENT_ARRAY_BUFFER
#define ME_VBO_VERTEX  GL_ARRAY_BUFFER

///simple out of util
#ifdef DEBUG
#define ME_SIV(msg,rsg) Util::InvokeConsole(msg,true,__FUNCTION__,(long)this + rsg)
#define ME_SIVD(msg,rsg) Util::InvokeConsole(msg,true,__FUNCTION__,rsg)
#else
#define ME_SIV(msg,rsg)
#define ME_SIVD(msg,rsg)
#endif // DEBUG

///OpenGL supports
#define EXT_AF "GL_EXT_texture_filter_anisotropic"

#define ME_FRAME_ADJUST_V 0.8

#ifndef PI
    #define PI 3.1415926
#endif // PI

#ifndef rad2deg
    #define rad2deg(r) (180*((r)/PI))
#endif // rad2deg

#ifndef deg2rad
    #define deg2rad(d) (((d)/180)*PI)
#endif // deg2rad

namespace me{
    using namespace std;

    class Program{
    public:

    private:

    };
///Utils
    //a simple performance counter
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


    class Util{
    public:
        ///created for InvokeConsole
        static unordered_set<string> sessions;
        static bool initedGlew;
        static bool initedGLFW;

        //Get OpenGL errors,appender is a string used to append the errors,sig is the prefix of the line
        //Returns whether there exists opengl errors
        static bool GetOpenGLError(string&appender,const char * sigStr = "Find OpenGL Error:");
        //print opengl errors if exists
        static void PrintOpenGLError();
        //get the size of a file,given the file path
        static size_t file_size(const char*filename);
        //init gl extension wrangler
        static int InitGlew();
        //init gl framework
        static void InitGLFW(int major = 4,int minor = 3);
        //given the shader id,append the log after appender,returns...
        static bool GetShaderLog(GLuint shader,string&appender);
        //print the shader logs if extists
        static void PrintShaderLog(GLuint shader);
        //get the whole program's log
        static bool GetProgramLog(GLuint prog,string&appender);
        //print the program's log if exists
        static void PrintProgramLog(GLuint prog);
        //register terminal function,used to free glfw resources
        static void RegisterTerminal();
        static void OnTerminal();
        //invoke an error to the console,requires define DEBUG macros!
        static void InvokeConsole(const char * s,bool onlyOnce = false,const char * sessionId = NULL,long = 0);

        ///prevent instancing
        Util() = delete;
        ~Util() = delete;
        void operator=(Util&) = delete;
    };

///OpenGL
    struct Vertex{
        glm::vec3 pos;
        glm::vec2 texCoord;
        glm::vec4 color;

        bool operator==(const Vertex& ano) const;
    };

    class VBO{
    public:
        unsigned int vbo_type;
        GLuint drawMethod;
        //vertices per shape
        unsigned int tps;

        VBO(GLuint v = 0,unsigned int vbo_type = ME_VBO_VERTEX);

        vector<GLfloat>* operator=(vector<GLfloat>& v);
        void Set(vector<GLfloat> v);
        void Set(GLfloat *d,size_t sz);

        void bind();
        void bind2(GLuint index);

        GLuint GetVBO();
        GLuint CreateNew();

        static void AttributePointer(GLuint bindingIndex,GLuint typesPerI = 3,GLenum type = GL_FLOAT,GLboolean normalized = GL_FALSE,GLsizei stride = 0,const void * pointer = NULL);
        static void EnableArray(GLuint index);
    private:
        friend class VBOs;
        void SetVBO(GLuint v);
        GLuint vbo;
    };

    class VBOs{
    public:
        VBO& operator[](unsigned int index);
        vector<VBO> GetVBOs();
        vector<GLuint> GetGLVBOs();
        void AppendVBOs(unsigned int count,unsigned int tp = ME_VBO_VERTEX);
        VBOs();
    private:
        vector<VBO> vbos;
    };

///Game
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

        void BuildMV(glm::mat4 * m);
        void BuildMV(GObject * g);

        void BindVBO(VBO vvbo,VBO cvbo = VBO(0));
        VBO GetVBO();

        void SetMovement(bool = true);

        void SetBindings(unsigned int vb = 0,unsigned int cb = 1);

        void Rotate(float x,float y,float z = 0);


        bool movement;

        glm::qua<float> rotations;
        glm::vec3 position;

        glm::mat4 mvmat;
        glm::mat4 mat;
        glm::mat4 rmat;

        glm::mat4 tempMat;

        glm::vec3 left;
        glm::vec3 forward;
        glm::vec3 up;

        unsigned int vbind;
        unsigned int cbind;
    private:
        friend class Window;
        VBO vbo;
        VBO coord;
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

    struct Model{
        vector<Vertex> vertices;
        vector<unsigned int> indices;
//        vector<glm::vec3> normals;
        int LoadModelFromFile(const char * fname);
//        int LoadModelFromMem(const char * data,bool enableNormals = true,bool enableIndices = true);
        void UploadToOpenGL();

        void CreateVBOs();

        void SetBindings(GLuint vertex);

        void GenBuffers();

        ~Model();
        Model();

        float * vbuf;
        unsigned int vsz;
        float * ibuf;
        unsigned int isz;

        VBO vvbo,ivbo;
    };

///Shaders
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
        Shader(bool initProgram = false);
        //create the shader program if program is not inited at constrcution
        void CreateProgram();
        //get uniform
        ShaderArg GetUniform(const char * s);
        ShaderArg GetUniform(const string & s);
        ShaderArg operator[](const char * s);
        ShaderArg operator[](string & s);
        ///load shaders
        //load&link&log
        int LoadLinkLogF(const char * vert,const char * frag,const char * geo = NULL);
        int LoadLinkLogM(const char * vert,const char * frag,const char * geo = NULL);
        //only load
        int LoadFromFile(const string&file,GLenum type);
        int LoadFromFile(const char * file,GLenum type,size_t sz = ME_DETECT_SIZE);
        int LoadFromMemory(const string&str,GLenum type);
        int LoadFromMemory(const char * str,GLenum type,size_t sz = ME_DETECT_SIZE);
        //load many
        int LoadsFromFile(const char * vert,const char * frag,const char * geometry = NULL);
        int LoadsFromMem(const char * vert,const char * frag,const char * geometry = NULL);
        //get ids,returns number below 0 if target isn't available
        int GetVertexShader();
        int GetFragmentShader();
        int GetGeometryShader();
        //returns 0 if program hasn't been created
        GLuint GetProgram();

        //bind shader
        void bind();
        //bind shader,pass NULL to unbind
        static void bind(Shader*);
        //unbind
        static void unbind();

        //link shader
        void LinkShader();
        //print logs
        void Log();
        //get logs
        string GetLog();
        //append logs
        void StoreLog(string& appender);
    private:
        friend class Window;
        GLuint program;
        GLuint vertex,fragment,geometry;
        bool enabled[ME_SHADER_TYPEC];
    };

///Graphics
    class Texture{
    public:
        Texture();
        //free data
        ~Texture();
        int LoadFromFile(const char * f);
        ///if copy == true,sz isn't effective
        int LoadFromMem(unsigned char * d,size_t sz,bool copy = true);
        int UploadToOpenGL(bool genMipMap = true,int rtp = GL_REPEAT);
        GLuint GetHandle();
        void Activate(GLuint index);
    private:
        unsigned char * data;
        GLuint handle;
        unsigned int width,height;
        unsigned int channels;
        bool deleteS;
    };

    struct GLSupport{
        GLSupport() = delete;
        void operator=(const GLSupport&) = delete;

        enum GLType{
            AnisotropicFilter
        };

        static bool Check(GLType tp);
        static bool Enable(GLType tp,float v);
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

        void MakeCurrent();
        unsigned int GetFramerateLimit();
        void SetFramerateLimit(unsigned int limit);

        void Draw(GObject&,GLuint targetC,GLuint instance = 1);
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

#endif // ME_H_INCLUDED
