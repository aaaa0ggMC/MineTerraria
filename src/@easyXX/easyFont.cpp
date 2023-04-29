#include "easyFont.h"
#include <iostream>

using namespace std;
using namespace sf;

static Font * dfont;

int MegaFont::loadDefault(){
    dfont = new Font();
    if(!dfont){
        return -1;
    }
    if(!dfont->loadFromFile(DEFAULT_FONT_RES_PTH)){
        return -2;
    }
    return 0;
}

MegaFont::MegaFont(const string & path,bool longAlive){
    font = new Font();
    this->longAlive = longAlive;
    if(font){
        if(!font->loadFromFile(path)){
            //Can't load,then replace
            delete font;
            font = dfont;
        }
    }else{
        font = dfont;
    }
}

MegaFont::MegaFont(Font * font,bool longAlive,bool defaultFont){
    this->longAlive = longAlive;
    this->font = font;
    this->isdef = defaultFont;
}

MegaFont::~MegaFont(){
    if(font){
        delete font;
        font = NULL;
    }
    if(isdef){
        dfont = NULL;
    }
    if(dfont && longAlive && !isdef){
        delete dfont;
        dfont = NULL;
    }
}

Font * MegaFont::getDefaultFont(){
    return dfont;
}
