#ifndef EASYTEX_H_INCLUDED
#define EASYTEX_H_INCLUDED
#include "../kernel.h"

using namespace std;
using namespace sf;

class TexturesHelper{
public:
    map<string,Texture *> textureList;
    ~TexturesHelper();
    Texture * operator[](string key);
    int add(string key,Texture * dest);
};

#endif // EASYTEX_H_INCLUDED
