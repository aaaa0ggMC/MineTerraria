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
#include <thread>
#include <stdlib.h>
#include <omp.h>
#include <unordered_map>
#include <functional>

#include <MultiEnString.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_IMAGE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

///macros
//arg pass
#define ME_DETECT_SIZE 0

//Errors
#define ME_ENO_ERROR                          0b0
#define ME_EOPENGL_ERROR                      0b1
#define ME_EEMPTY_STRING                      0b10
#define ME_EBAD_IO                            0b100
#define ME_EBAD_MEM                           0b1000
#define ME_EBAD_TYPE                          0b10000
#define ME_ENO_DATA                           0b100000
#define ME_EALREADY_EXI                       0b1000000
#define ME_EFONT_BAD_FREETYPE                 0b10000000
//shaders
#define ME_SHADER_VERTEX GL_VERTEX_SHADER
#define ME_SHADER_FRAGMENT GL_FRAGMENT_SHADER
#define ME_SHADER_GEOMETRY GL_GEOMETRY_SHADER
#define ME_SHADER_TYPEC 3 //available shader types
//vbos
#define ME_VBO_ELEMENT GL_ELEMENT_ARRAY_BUFFER
#define ME_VBO_VERTEX  GL_ARRAY_BUFFER
//implement opengl
#define ME_CW GL_CW
#define ME_CCW GL_CCW
//fonts
#define ME_FONT_ATTR_PARENT      0b1
#define ME_FONT_ATTR_ITALIC      0b10
#define ME_FONT_ATTR_BOLD        0b100
#define ME_FONT_ATTR_UNDERLINE   0b1000
#define ME_FONT_ATTR_DELETELINE  0b10000
#define ME_FONT_ATTR_PERMANENT   0b100000

///Tools
#define ME_BOLD_OFFSET 16
#define ME_BOLD(X,Y) (unsigned int)((Y << ME_BOLD_OFFSET) + X)
#define ME_BOLD_MASK 0xffff

#define ME_FONTSIZE_OFFSET 16
#define ME_FONTSIZE(X,Y) (unsigned int)((Y << ME_FONTSIZE_OFFSET) + X)
#define ME_FONTSIZE_MASK 0xffff

#define ME_FONT_OUTLINE_NUM_POINTS 256
#define ME_FONT_OUTLINE_NUM_CONTOURS 128

//ignore 0 --- 31
#define ME_FREQ_LOW 5

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
    struct fGlFont;
    struct CharProperty;
}
namespace std {
    template <>
    struct hash<me::CharProperty> {
        size_t operator()(const me::CharProperty& cp) const;
    };
}

namespace me{
    using namespace std;

    class noncopyable
    {
     public:
      noncopyable(const noncopyable&) = delete;
      void operator=(const noncopyable&) = delete;
     protected:
        noncopyable() = default;
        ~noncopyable() = default;
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

    class Changer{
    public:
        int dirty;
        void MarkDirty(int mask=1);
        int GetDirty();
        bool Match(int mask = 1);
        Changer();
        virtual void Update(unsigned int=0) = 0;
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
        unsigned int stride;

        VBO(GLuint v = 0,unsigned int vbo_type = ME_VBO_VERTEX,unsigned int=0);

        vector<GLfloat>* operator=(vector<GLfloat>& v);
        vector<GLint>* operator=(vector<GLint>& v);
        void Set(vector<GLfloat> v);
        void Set(GLfloat *d,size_t sz);
        void Set(vector<GLint> v);
        void Set(GLint *d,size_t sz);

        void bind();
        void bind2(GLuint index);

        void unbind();

        GLuint GetVBO();
        GLuint CreateNew();

        static void AttributePointer(GLuint bindingIndex,GLuint typesPerI = 3,GLenum type = GL_FLOAT,GLboolean normalized = GL_FALSE,GLsizei stride = 0,const void * pointer = NULL);
        static void EnableArray(GLuint index);
    private:
        friend class VBOs;
        void SetVBO(GLuint v);
        GLuint vbo;
        unsigned int bidx;
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

    ///GObjects Changer Defines
    #define ME_GROT 0x00000001
    #define ME_GMOD 0x00000010

    class GObject : public Changer{
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

        void Update(unsigned int=0);

        void SetMovement(bool = true);

        void SetBindings(unsigned int vb = 0,unsigned int cb = 1);

        void Rotate(float x,float y,float z = 0);
        void SetRotation(float x,float y,float z = 0);

        void Scale(float x,float y,float z);
        void SetScale(float x,float y,float z);

        void UpdateRotationMat();

        bool movement;

        glm::vec3 rotations;
        glm::vec3 position;
        glm::vec3 scale;

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
        friend class Model;
        VBO vbo;
        VBO coord;
    };

    class Camera : public GObject{
    public:
        glm::mat4 perspec;
        glm::mat4 vrp_matrix;
        bool updateVRP;

        Camera(float x = 0,float y = 0,float z = 0,bool = true,bool = true);
        void UpdateModelMat();
        void BuildPerspec(float fieldOfView,float ratio,float nearPlane,float farPlane);
        void BuildPerspec(float fieldOfView,float width,float height,float nearPlane,float farPlane);
        void BuildPerspec(float fieldOfView,void*w,float nearPlane,float farPlane);
        void BuildOrth(float left,float right,float bottom,float top,float znear = 0.1,float zfar = 1000);
        void BuildOrthA(float left,float top,float xlen,void*w,float znear = 0.1,float zfar = 1000);
        void BuildOrthA(float left,float top,float xlen,float aspectRatio,float znear = 0.1,float zfar = 1000);
    };
    ///Model
    class ObjLoader{
    public:
        ObjLoader(vector<float>&vf,vector<float>& nm,vector<float>& tc,unsigned int& fc);
        bool LoadFromObj(const char * obj);
        bool LoadFromStlBin(const char * fp);

        vector<glm::vec3> vertices;
        vector<glm::vec3> normals;
        vector<glm::vec2> tcoords;

        vector<float>& vfloats;
        vector<float>& vnormals;
        vector<float>& vtexc;

//        vector<int> vindices;
//        vector<int> nindices;
//        vector<int> tindices;

        unsigned int& vertc;
    };
    struct Model : public GObject{
//        VBO ivbo;
        VBO nvbo;
        vector<float> vfloats;
//        vector<int> indices;
        vector<float> nfloats;
        vector<float> tfloats;
        unsigned int vertc;
        bool hasNormal;

        int LoadModelFromObj(const char * fname);
        int LoadModelFromStlBin(const char * fname);
        void UploadToOpenGL();
        void CreateVBOs(VBO& v,VBO& n);
        void SetBindings(GLuint vertex,GLuint);
        void Unbind();
        void SetData(vector<float>*vert,vector<float>*texc=NULL,vector<float>*norm=NULL);

        Model(float x,float y,float z = 0);
    };

    struct Model1 : public GObject{
        Model1(float x=0,float y=0,float z=0);

        void BindVBos(VBO vert,VBO indi,VBO texc = VBO(),VBO norm = VBO());
    };

///Shaders
    class ShaderArg {
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
        glm::mat4& operator=(glm::mat4& v);
        glm::mat3& operator=(glm::mat3& v);
        glm::vec4& operator=(glm::vec4& v);
        glm::vec3& operator=(glm::vec3& v);
        glm::vec2& operator=(glm::vec2& v);

        GLdouble UploadDouble(GLdouble v);
        GLint UploadInt(GLint v);
        GLfloat* UploadVec4(GLfloat *);
        GLfloat* UploadVec3(GLfloat *);
        GLfloat* UploadVec2(GLfloat *);
        glm::vec4& UploadVec4(glm::vec4&);
        glm::vec3& UploadVec3(glm::vec3&);
        glm::vec2& UploadVec2(glm::vec2&);
        void UploadVec4(float x,float y,float z,float w);
        void UploadVec3(float x,float y,float z);
        void UploadVec2(float x,float y);
        GLfloat* UploadMat3(GLfloat *);
        glm::mat3& UploadMat3(glm::mat3&);
        void UploadRaw(glm::mat4);
        void UploadRaw(glm::mat3);
        void UploadRaw(glm::vec4);
        void UploadRaw(glm::vec3);
        void UploadRaw(glm::vec2);
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
    class Texture : public noncopyable{
    public:
        friend class GlFont;
        Texture();
        //free data
        ~Texture();
        int LoadFromFile(const char * f);
        ///if copy == true,sz isn't effective
        int LoadFromMem(unsigned char * d,size_t sz,bool copy = true);
        int Create2DTextureArray(unsigned int width,unsigned int height,unsigned int depth,GLuint fmt = GL_RGBA,GLuint sourcefmt = GL_RED,unsigned char * source = 0);
        int CreateTextureBuffer(unsigned int size,unsigned char * init = NULL);
        int UpdateGLTexture1D(unsigned char * bytes,unsigned int size,unsigned int offset);
        int UpdateGLTexture(unsigned char * bytes,unsigned int depth_offset,unsigned int w,unsigned int h,unsigned int depth = 1,GLuint format = GL_RGBA,unsigned int alignValue = 4,bool clearTex = false);
        int ClearGLTexture(unsigned int start_off_contained = 0,unsigned int end_off_contained = 0,float r = 0,float g = 0,float b = 0,float a = 0);
        int UploadToOpenGL(bool genMipMap = true,int rtp = GL_REPEAT,unsigned int minft = GL_LINEAR_MIPMAP_NEAREST,unsigned int maxft = GL_LINEAR_MIPMAP_NEAREST);
        GLuint GetHandle();
        void Activate(GLuint index);
        static void Deactivate(GLuint);
        void FreeData();
        void FreeTexture();

        static void InitFramebuffer();

        unsigned char * data;
        GLuint handle;
        GLenum type;

        unsigned int width,height;
        unsigned int depth;
        GLuint format;
        unsigned int channels;

        bool deleteS;
        bool dataAva;

        ///Used to clear texture data
        static GLuint frameBuffer;
        static bool inited;
    };

    struct Light{
        glm::vec4 color;
        float strength;
    };

    struct GlobalLight : public Light{};

    struct DotLight : public Light{
        glm::vec3 position;
    };

    struct Material{
        glm::vec4 color;
        float shiness;
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

    class Window : public noncopyable{
    public:
        typedef void (*WPaintFn)(Window&,double currentTime,Camera*cam);
        typedef void (*OnKeyPress)(Window&,double elapseus,Camera*cam);
        typedef void (*DiscreteKeyListener)(GLFWwindow * win,int key,int scancode,int action,int mods);

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

        void PollEvents();

        GLFWwindow * GetGLFW();
        long GetSystemHandle();

        void MakeCurrent();
        unsigned int GetFramerateLimit();
        void SetFramerateLimit(unsigned int limit);

        void SetCheckInputInterval(unsigned int interval = 120);

        void Draw(GObject&,GLuint targetC,GLuint instance = 1);
        void DrawModel(Model & model,GLuint instance = 1,GLuint bindingIndex = 0,GLuint=2);

        void SetPaintFunction(WPaintFn);
        void OnKeyPressEvent(OnKeyPress);
        void SetDiscreteKeyListener(DiscreteKeyListener l);
        void UseCamera(Camera& cam);

        void EnableDepthTest(GLenum = GL_LEQUAL);
        void DisableDepthTest();

        void EnableCullFaces();
        void DisableCullFaces();

        void SetFrontFace(unsigned int dr);

        bool KeyInputed(int key,int state = GLFW_PRESS);

        void SetUIRange(float left,float top,float right,float bottom);

        void __Display_Pre();
        void __Display_Aft();

        glm::vec2 GetWindowSize();
        glm::vec2 GetBufferSize();

        Camera uiCam;
    private:
        static Window * current;
        std::thread intervalThread;
        GLFWwindow* win;
        WPaintFn paint;
        OnKeyPress press;
        DiscreteKeyListener keyListener;
        unsigned int flimit;
        float twait,frame_start;
        unsigned int interval;
        bool limitedF;
        Camera * curCam;
        float firstTime;
        bool isOpen;


    };
///After:: Math
    class Velocity{
    public:
        float v;
        glm::vec3 vv;
        void New();
        void Add(int,int,int);
        void Form();

        void SetVelocity(float v);
        void Move(GObject & obj,float etime);
        void MoveDr(GObject & obj,float etime);

        Velocity(float v);
    };
///Progarm
    class Program{
    public:
        vector<GObject*> objs;
        void PushObj(vector<GObject*> l);
        void Update();
    private:

    };

    #ifdef ME_BUILD_PREFABS
    namespace prefab{

    }
    #endif // ME_BUILD_PREFABS

///Font & Text
    class MemFont : public noncopyable{
    public:
        static FT_Library library;
        static bool inited;
        static FT_Matrix italicMatrix;

        FT_Face face;

        ///Helpers
        unsigned int step;

        unsigned int attribute,font_sizexy;
        unsigned int bold_strengthxy;

        MemFont(unsigned int font_sizexy = ME_FONTSIZE(0,48),unsigned int def_atrribute = 0,unsigned int bold_strengthxy = ME_BOLD(128,128));
        ~MemFont();

        int LoadFromFile(const char * filePath,unsigned int face_index = 0);
        int LoadFromMem(unsigned char * buffer,unsigned long size,unsigned int face_index= 0);

        FT_GlyphSlot LoadChar(FT_ULong charcode_gbk,unsigned int attri = ME_FONT_ATTR_PARENT,FT_Render_Mode_ mode = FT_RENDER_MODE_NORMAL,bool render = true);
        FT_GlyphSlot LoadCharEx(FT_ULong charcode_gbk,unsigned int font_sizexy,unsigned int attri = ME_FONT_ATTR_PARENT,FT_Render_Mode_ mode = FT_RENDER_MODE_NORMAL,bool render = true);

        void GenChar(FT_Glyph & target,FT_ULong charcode_gbk,unsigned int attri = ME_FONT_ATTR_PARENT,FT_Render_Mode_ mode = FT_RENDER_MODE_NORMAL,bool render = true);

        void SetDefSize(unsigned short font_sizex,unsigned short font_sizey);
        void SetDefAttribute(unsigned int attribute);
        void SetDefBoldStrength(unsigned short bx,unsigned short by);

        void SetFormat(unsigned int att);
    };

    ///CharProperty
    struct CharProperty{
        FT_ULong charcode;
        unsigned int attribute;
        unsigned int bold_strength;
        unsigned int font_size;
    };
    ///Char Data
    struct CharData{
        unsigned int offset;
        unsigned int width;
        unsigned int height;
    };

    ///fGlFont
    class fGlFont : public noncopyable{
    public:
        MemFont & memfont;
        Texture buffer;
        unsigned char * bufferInCPU;

        unordered_map<CharProperty,CharData> charcodes;

        unsigned int allocate;



        ///returns offset
        unsigned int LoadCharGB2312(FT_ULong charcode);
        CharData LoadCharGB2312Ex(FT_ULong charcode,unsigned int font_size,unsigned int attri,unsigned int bold_strenth);


        fGlFont(MemFont&,unsigned int allocate);
        ~fGlFont();
    };

    class GlFont : public noncopyable{
    public:
        MemFont& memfont;
        Texture buffer;
        FT_ULong * charcodes_gb;
        unsigned long * frequencies;
        unsigned int * attributes;
        FT_GlyphSlot defGlyph;


        unsigned int width,height,depth;
        ///a index that determines the permanence of the elements,id bigger than this will be detected
        int div_line_permanent;


        GlFont(MemFont& memfont,unsigned int width = 0,unsigned int height = 0,unsigned int depth = 0,unsigned int font_sizexy = ME_FONTSIZE(0,48),unsigned int def_atrribute = 0,unsigned int bold_strengthxy = ME_BOLD(4,4));
        ~GlFont();

        void SetSize(unsigned short font_sizex,unsigned short font_sizey);
        void SetAttribute(unsigned int attribute);
        void SetBoldStrength(unsigned short bx,unsigned short by);
        int SetBufferSize(unsigned int w,unsigned int h,unsigned int d);

        int CreateBuffer();

        unsigned int LoadCharGB2312(FT_ULong charcode_gb);
        unsigned int LoadCharUnicode(FT_ULong charcode_un);
        unsigned int LoadCharUTF8(FT_ULong charcode_u8);

        void _LoadCharData(FT_ULong charcode_gb,unsigned int index);
        void _FreeCharData(unsigned int index);
        void _UpdateOpenGL(unsigned int index);

        ///Release all frequencies
        void ReleaseCache();
    };
}

#endif // ME_H_INCLUDED
