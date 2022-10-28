#include "easyTex.h"

using namespace sf;
using namespace std;

int TexturesHelper::add(string key,Texture * dest){
    textureList.insert(std::make_pair(key,dest));
    return 0;
}
Texture * TexturesHelper::operator[](string key){
    map<string,Texture*>::iterator iter = textureList.find(key);
    if(iter == textureList.end())return NULL;
    return iter->second;
}
TexturesHelper::~TexturesHelper(){
    for(map<string,Texture*>::iterator iter = textureList.begin(); iter != textureList.end(); iter++) {
        delete (Texture*)(iter->second);
    }
    textureList.clear();
}
