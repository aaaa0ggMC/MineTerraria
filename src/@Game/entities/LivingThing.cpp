#include "LivingThing.h"

using namespace game;

FloatRect LivingThing::GenMoveCollider(float x,float y){
    return FloatRect(position.x + x - cptg.x / 2,position.y + y - cptg.y/4,cptg.x,cptg.y/4);
}


FloatRect LivingThing::GetRect(){
    return GenMoveCollider(veclocity.x,veclocity.y);
}

Pt2D<float> LivingThing::GetFormedScale(float baseSize){
    return {baseSize / texSz.x * cptg.x,baseSize / texSz.y * cptg.y};
}
