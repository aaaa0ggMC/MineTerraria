#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_DDS
#define STBI_NO_PVR
#define STBI_NO_PKM
#define STBI_NO_QOI
#define STBI_NO_EXT
#include <stb_image.h>
#include <string.h>

using namespace me;
using namespace std;

Texture::Texture(){
    data = NULL;
    handle = 0;
    channels = width = height = 0;
    deleteS  = false;
}

int Texture::LoadFromFile(const char * f){
    if(data){
        ME_SIV("data already created!",2);
        return ME_ALREADY_EXI;
    }
    if(!f){
        ME_SIV("given [NULL]",0);
        return ME_NO_DATA;
    }
    size_t sz = strlen(f);
    if(!sz){
        ME_SIV("given \\0",1);
        return ME_EMPTY_STRING;
    }
    sz = Utility::file_size(f);
    if(!sz){
        ME_SIV("empty file!",2);
        return ME_BAD_IO;
    }
    int w,h,ncs;
    unsigned char * d = stbi_load(f,&w,&h,&ncs,0);
    if(!d){
        string str = "Can't load file ";
        str += f;
        ME_SIV(str.c_str(),3);
        return ME_BAD_IO;
    }
    data = d;
    channels = ncs;
    width = w;
    height = h;
    deleteS = true;
    return ME_NO_ERROR;
}

int Texture::LoadFromMem(unsigned char * d,size_t sz,bool copy){
    if(data){
        ME_SIV("data already created!",2);
        return ME_ALREADY_EXI;
    }
    if(!d){
        ME_SIV("given [NULL]",0);
        return ME_NO_DATA;
    }
    if(copy && !sz){
        ME_SIV("If you want to copy the data,please give the size of the chunk.",1);
        return ME_BAD_MEM;
    }
    if(copy){
        data = new unsigned char[sz];
        memcpy(data,d,sz);
    }else data = d;
    return ME_NO_ERROR;
}

int Texture::UploadToOpenGL(bool gmm,int rtp){

    if(!data){
        ME_SIV("You DID NOT load the data!",0);
        return ME_NO_DATA;
    }

    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, rtp);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, rtp);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    if(gmm)glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D,0);

    return ME_NO_ERROR;
}

GLuint Texture::GetHandle(){
    if(handle)return handle;
    ME_SIV("You didn't call UploadToOpenGL to upload the texture!",0);
    return 0;
}

void Texture::Activate(GLuint index){
    if(index >= 32){
        ME_SIV("only supports index below 32",0);
        return;
    }
    glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D,handle);
}

Texture::~Texture(){
    if(deleteS){
        stbi_image_free(data);
    }else delete [] data;
}
