#ifndef EASYFONT_H_INCLUDED
#define EASYFONT_H_INCLUDED
#include "../kernel.h"
using namespace sf;
using namespace std;

#define megaTexLFF(tex,pth) if(!tex.loadFromFile(pth))

class MegaFont{
public:
    Font * font;
    bool longAlive;
    bool isdef;
    static int loadDefault();
    static Font * getDefaultFont();
    MegaFont(const string & path,bool longAlive = true);
    MegaFont(){}
    MegaFont(Font * font,bool longAlive = true,bool defaultFont = false);
    ~MegaFont();
};

#endif // EASYFONT_H_INCLUDED
