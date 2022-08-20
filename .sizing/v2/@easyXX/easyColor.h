#ifndef EASYCOLOR_H_INCLUDED
#define EASYCOLOR_H_INCLUDED
#include "../kernel.h"
using namespace sf;

#define rgba(r,g,b,a) Color(r,g,b,a)
#define dirt          Color(237,189,101)

Color SetAlpha(Color origin,int alpha);
Color ColorMoreXX(Color cl,float mul);

#endif // EASYCOLOR_H_INCLUDED
