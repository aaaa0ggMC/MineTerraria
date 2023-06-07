#ifndef VERTEXOBJECTS_H_INCLUDED
#define VERTEXOBJECTS_H_INCLUDED
#include <GL/glew.h>
#include <vector>

namespace me{
    using namespace std;

    class VBO{
    public:
        VBO();
        vector<GLfloat>* operator=(vector<GLfloat>& v);
        void Set(vector<GLfloat> v);
        void Set(GLfloat *d,size_t sz);
        void bind();
        static void AttributePointer(GLuint bindingIndex,GLuint typesPerI = 3,GLenum type = GL_FLOAT,GLboolean normalized = GL_FALSE,GLsizei stride = 0,const void * pointer = NULL);
        static void EnableArray(GLuint index);
        void BindingTo(GLuint index);
        GLuint GetVBO();
    private:
        friend class VBOs;
        VBO(GLuint v);
        void SetVBO(GLuint v);
        GLuint vbo;
    };

    class VBOs{
    public:
        VBO operator[](unsigned int index);
        vector<VBO> GetVBOs();
        vector<GLuint> GetGLVBOs();
        void AppendVBOs(unsigned int count);
    private:
        vector<VBO> vbos;
    };

    class VertexArrayObject{
    public:

    private:
        vector<VBOs> vbos;
    };
}


#endif // VERTEXOBJECTS_H_INCLUDED
