#include "sfml-layout.h"

using namespace std;
using namespace sf;


Vector2f setPosRelative(FloatRect rct,Vector2i winsz,PosRelative hor,PosRelative ver,float horPer,float verPer){
    float width = rct.width;
    float height = rct.height;
    float x = rct.left;
    float y = rct.top;
    //Vertical changing
    if(ver == PosCenter){
        y = ((float)winsz.y - height)/(float)2;
    }else if(ver == PosPercent){
        y = verPer * winsz.y;
    }else if(ver == PosLeftUp || hor == PosLeftUpShow){
        y = 0;
    }else if(ver == PosRightDown){
        y = winsz.y;
    }else if(ver == PosRightDownShow){
        y = winsz.y - height;
    }//Not change cause PosNotChange

    //Horizontal changing
    if(hor == PosLeftUp || hor == PosLeftUpShow){
        x = 0;
    }else if(hor == PosRightDown){
        x = (float)winsz.x;
    }else if(hor == PosRightDownShow){
        x = winsz.x - width;
    }else if(hor == PosCenter){
        x = ((float)winsz.x - width)/(float)2;
    }else if(hor == PosPercent){
        x = horPer * winsz.x;
    }//Else do nothing,Not change PosNotChange

    return Vector2f(x,y);
}

Vector2f appendPixel(FloatRect rct,float xm,float ym){
    float x = rct.left;
    float y = rct.top;
    float width = rct.width;
    float height = rct.height;

    return Vector2f((x+width+xm),(y+height+ym));
}
