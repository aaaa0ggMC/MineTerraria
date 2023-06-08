///@Copyright aaaa0ggmc 2023
#ifndef VERTEXOBJECTS_H_INCLUDED
#define VERTEXOBJECTS_H_INCLUDED
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

namespace me{
    using namespace std;

    class VBO{
    public:
        //~VBO();
        VBO();
        vector<GLfloat>* operator=(vector<GLfloat>& v);
        void Set(vector<GLfloat> v);
        void Set(GLfloat *d,size_t sz);
        void bind();
        static void AttributePointer(GLuint bindingIndex,GLuint typesPerI = 3,GLenum type = GL_FLOAT,GLboolean normalized = GL_FALSE,GLsizei stride = 0,const void * pointer = NULL);
        static void EnableArray(GLuint index);
        void BindingTo(GLuint index);
        GLuint GetVBO();
        GLuint CreateNew(bool isebo = false);

        GLuint drawMethod;

        bool isebo;

    private:
        friend class VBOs;
        //float * buf;
        VBO(GLuint v);
        void SetVBO(GLuint v);
        GLuint vbo;
    };

    class VBOs{
    public:
        VBO& operator[](unsigned int index);
        vector<VBO> GetVBOs();
        vector<GLuint> GetGLVBOs();
        void AppendVBOs(unsigned int count);
        VBOs();
    private:
        vector<VBO> vbos;
    };

    struct Vertex{
        glm::vec3 pos;
        glm::vec2 texCoord;
//        glm::vec4 color;

        bool operator==(const Vertex& ano) const;
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

    class VertexArrayObject{
    public:

    private:
        vector<VBOs> vbos;
    };
}


#endif // VERTEXOBJECTS_H_INCLUDED
