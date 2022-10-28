#ifndef SFML_LAYOUT_H_INCLUDED
#define SFML_LAYOUT_H_INCLUDED
#include "../kernel.h"

using namespace sf;

enum PosRelative{
    PosCenter,
    PosLeftUp,
    PosRightDown,
    PosPercent,
    PosNotChange,
    PosLeftUpShow,//Assure to show object
    PosRightDownShow
};


//Hor横向方向 ver竖直方向
Vector2f setPosRelative(FloatRect rct,Vector2i winsz,PosRelative hor,PosRelative ver,float horPer = 0,float verPer = 0);
Vector2f appendPixel(FloatRect rct,float xm,float ym);

#endif // SFML_LAYOUT_H_INCLUDED
