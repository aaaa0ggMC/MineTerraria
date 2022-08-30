#include "easyColor.h"

using namespace sf;

Color SetAlpha(Color origin,int alpha){
    return Color(origin.r,origin.g,origin.b,alpha);
}

Color ColorMoreXX(Color cl,float mul){
    return Color(cl.r * mul,cl.g * mul,cl.b * mul);
}
