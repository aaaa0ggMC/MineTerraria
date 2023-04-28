#ifndef EASYTEX_H_INCLUDED
#define EASYTEX_H_INCLUDED
#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <unordered_map>

using namespace std;
using namespace sf;

class TexturesHelper{
public:
    unordered_map<string,Texture *> textureList;
    ~TexturesHelper();
    Texture * operator[](string key);
    int add(string key,Texture * dest);
};

#endif // EASYTEX_H_INCLUDED
