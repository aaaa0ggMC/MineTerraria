#include "player.h"

using namespace game;
using namespace sf;

FloatRect Player::GenMoveCollider(float x,float y){
    return FloatRect((x + this->position.x - _f(texSz.x) / AbstractTile::len / 2),(y+this->position.y - _f(texSz.y) / AbstractTile::len /2),_f(texSz.x) / AbstractTile::len,_f(texSz.y) / AbstractTile::len);
}
