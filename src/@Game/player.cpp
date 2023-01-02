#include "player.h"

using namespace game;
using namespace sf;

FloatRect Player::GenMoveCollider(float x,float y){
    return FloatRect(position.x + x - cptg.x / 2,position.y + y - cptg.y/4,cptg.x,cptg.y/4);
}

Pt2D<float> Player::GetFormedScale(float baseSize){
    return {baseSize / texSz.x * cptg.x,baseSize / texSz.y * cptg.y};
}

FloatRect Player::GetRect(){
    return GenMoveCollider(veclocity.x,veclocity.y);
}
