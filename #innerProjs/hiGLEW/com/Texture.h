#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
#include "Utility.h"

namespace me{
    class Texture{
    public:
        Texture();
        ~Texture();
        int LoadFromFile(const char * f);
        ///���copy = true,��ôsz���Բ����ã���Ϊ0
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
}

#endif // TEXTURE_H_INCLUDED
