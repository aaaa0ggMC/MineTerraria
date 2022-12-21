#include "player.h"

using namespace game;
using namespace sf;

FloatRect Player::GenMoveCollider(float x,float y){
    return FloatRect((x + this->position.x)*AbstractTile::len,(y+this->position.y)*AbstractTile::len,texSz.x,texSz.y);
}
