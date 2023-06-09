///@Copyright aaaa0ggmc 2023
#include "VertexObjects.h"
#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include "Utility.h"

namespace std {
	template<> struct hash<me::Vertex> {
	size_t operator()(me::Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec2>()(vertex.texCoord))));
		}
	};
}

using namespace std;
using namespace me;
using namespace tinyobj;

VBO::VBO(){
    vbo = 0;
    drawMethod = GL_TRIANGLES;
    isebo = false;
    tps = 3;
//    buf = NULL;
}

VBO::VBO(GLuint v){
    vbo = v;
    drawMethod = GL_TRIANGLES;
    isebo = false;
    tps = 3;
//    buf = NULL;
}

void VBO::Set(GLfloat d[],size_t sz){
    if(!vbo || !sz)return;
    GLuint target = isebo?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER;
    glBindBuffer(target,vbo);
    glBufferData(target,sz,d,GL_STATIC_DRAW);
}

void VBO::Set(vector<GLfloat> v){
    (*this) = v;
}

bool Vertex::operator==(const Vertex& other) const{
	return pos == other.pos && texCoord == other.texCoord;
}

void VBO::SetVBO(GLuint v){vbo = v;}

GLuint VBO::GetVBO(){return vbo;}

VBOs::VBOs(){
    vbos.push_back(VBO(0));
}

[[deprecated("use float * instead")]] vector<GLfloat>* VBO::operator=(vector<GLfloat> & v){
    ME_SIV("Unavailable,use float * instead",0);
    return &v;
//    if(!vbo)return &v;
//    size_t sz = v.size() * sizeof(GLfloat);
////    GLfloat * buf = new GLfloat[sz];
////    for(unsigned int i =0;i < sz;++i){
////        buf[i] = v[i];
////    }
////    GLuint target = isebo?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER;
////    glBindBuffer(target,vbo);
////    glBufferData(target,sz,buf,GL_STATIC_DRAW);
////    this->buf = buf;
////    return &v;
}


void VBOs::AppendVBOs(unsigned int c){
    GLuint *gvbo = new GLuint[c];
    glGenBuffers(c,gvbo);
    for(unsigned int i = 0; i < c;++i){
        vbos.push_back(VBO(gvbo[i]));
    }
}

vector<GLuint> VBOs::GetGLVBOs(){
    vector<GLuint> uv;
    for(const VBO & va : vbos){
        uv.push_back(va.vbo);
    }
    uv.erase(uv.begin(),uv.begin()+1);
    return uv;
}

vector<VBO> VBOs::GetVBOs(){
    vector<VBO> ret = vbos;
    ret.erase(ret.begin(),ret.begin()+1);
    return ret;
}

VBO& VBOs::operator[](unsigned int index){
    if(index+1 >= vbos.size())return vbos[0];
    return vbos[index+1];
}

void VBO::bind(){
    GLuint target = isebo?GL_ELEMENT_ARRAY_BUFFER:GL_ARRAY_BUFFER;
    glBindBuffer(target,vbo);
}


void VBO::AttributePointer(GLuint index,GLuint typesPerI,GLenum type,GLboolean normalized,GLsizei stride,const void * pointer){
    glVertexAttribPointer(index,typesPerI,type,normalized,stride,pointer);
}

void VBO::EnableArray(GLuint index){
    glEnableVertexAttribArray(index);
}


void VBO::BindingTo(GLuint index){
    this->bind();
    if(isebo){
        ME_SIV("Element Buffer doesn't support being visited in shaders!",0);
        return;
    }
    AttributePointer(index,tps);
    EnableArray(index);
}

void KLoad(Model & m,vector<shape_t> & shapes,attrib_t & attrib){
    std::unordered_map<Vertex, uint32_t> uniqueVertices;
    for(const auto& shape : shapes) {
        for(const auto& index : shape.mesh.indices) {
            Vertex vertex;
            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                attrib.texcoords[2 * index.texcoord_index + 1]
            };

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(m.vertices.size());
                m.vertices.push_back(vertex);
                cout << "pushed" << endl;
            }
            m.indices.push_back(uniqueVertices[vertex]);
        }
    }
}


int Model::LoadModelFromFile(const char * fname){
    if(!fname){
        ME_SIV("given NULL",0);
        return ME_NO_DATA;
    }
    size_t sz = Utility::file_size(fname);
    if(!sz){
        ME_SIV("can't get file size",1);
        return ME_BAD_IO;
    }
    attrib_t attrib;
    vector<shape_t> shape;
    vector<material_t> matr;
    string err = "";
    if(!tinyobj::LoadObj(&attrib,&shape,&matr,&err,fname)){
        ME_SIV(err.c_str(),2);
        return ME_BAD_IO;
    }
    vertices.clear();
    indices.clear();
    KLoad(*this,shape,attrib);
    return ME_NO_ERROR;
}

GLuint VBO::CreateNew(bool e){
    if(vbo){
        ME_SIV("already created vbo!",0);
        return vbo;
    }
    glGenVertexArrays(1,&vbo);
    if(vbo){
        isebo = e;
        return vbo;
    }
    ME_SIV("gen vbo failed",1);
    return 0;
}

//int Model::LoadModelFromMem(const char * data,bool enableNormals,bool enableIndices){
//    ME_SIV("»¹Ã»×ö",0);
//    return ME_NO_ERROR;
//}

void Model::CreateVBOs(){
    vvbo.CreateNew();
    ivbo.CreateNew(true);
}

Model::Model(){
    ibuf = vbuf = NULL;
}

Model::~Model(){
    if(ibuf)delete [] ibuf;
    if(vbuf)delete [] vbuf;
}

void Model::GenBuffers(){
    vsz = vertices.size();
    vbuf = new float[vsz*3];
    for(unsigned int i = 0;i < vsz;++i){
        vbuf[i*3 + 0] = vertices[i].pos.x;
        vbuf[i*3 + 1] = vertices[i].pos.y;
        vbuf[i*3 + 2] = vertices[i].pos.z;
    }
    vsz *= 3;
    isz = indices.size();
    ibuf = new float[isz];
    for(unsigned int i = 0;i < isz;++i){
        ibuf[i] = indices[i];
    }
}

void Model::UploadToOpenGL(){
    if(!vvbo.GetVBO() || !ivbo.GetVBO()){
        ME_SIV("Some vbos are unavailable!",0);
        return;
    }
    GenBuffers();
    vvbo.Set(vbuf,vsz*sizeof(float));
    ivbo.Set(ibuf,isz*sizeof(float));
}

void Model::SetBindings(GLuint v){
    vvbo.BindingTo(0);
    ivbo.bind();
}
