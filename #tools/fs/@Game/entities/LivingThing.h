#ifndef LIVINGTHING_H_INCLUDED
#define LIVINGTHING_H_INCLUDED

#include "../../TerrarianNew.h"
#include "../Animations/ani.h"

using namespace sf;

namespace game{
    struct LivingThing{
        ///是图格地址，而不是屏幕地址
        Vector2f position;
        unsigned int dimension;
        Pt2D<float> cptg;//Collision Percentage
        Pt2D<float> veclocity;
        Pt2D<float> texSz;

        LivingThing(){
            texSz = {BASE_TILSZ,BASE_TILSZ*2};
            cptg = {0.5,1};
        }

        virtual LivingThing & setPosition(Vector2f p,uint ldimension){position = p;dimension = ldimension;return *this;}
        virtual LivingThing & setPosition(float x,float y,uint di){position.x = x;position.y = y;dimension = di;return *this;}
        virtual LivingThing & setCollisionPercen(Vector2f v){cptg = v;return *this;}
        virtual LivingThing & setCollisionPercen(float x,float y){return setCollisionPercen({x,y});}

        void Move(float x,float y){position.x += x;position.y += y;}
        FloatRect GenMoveCollider(float x,float y);
        FloatRect GetRect();
        Pt2D<float> GetFormedScale(float baseSize);
    };
}

#endif // LIVINGTHING_H_INCLUDED
