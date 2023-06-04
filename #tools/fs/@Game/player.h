#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "../TerrarianNew.h"
#include "entities/LivingThing.h"

using namespace std;
using namespace sf;

namespace game{
    class Player : public LivingThing{
    public:
        vector<Chunk*> rChunks;//dynamic loading chunks
        unsigned int rSize;//Streching size,0 means 1*1,1 means 3*3
        unsigned int uuid_local;

        Player & setPosition(Vector2f p,uint ldimension) override{position = p;dimension = ldimension;return *this;}
        Player & setPosition(float x,float y,uint di) override{position.x = x;position.y = y;dimension = di;return *this;}
        Player & setCollisionPercen(Vector2f v) override{cptg = v;return *this;}
        Player & setCollisionPercen(float x,float y) override{return setCollisionPercen({x,y});}
        Player & setBasicInformation(unsigned int uuid_local){this->uuid_local = uuid_local;return *this;}
        Player & setDyChunkInfo(uint rSize = 1){this->rSize = rSize;return *this;}
    };
}
/**
笔记： struct X : public Y中public 的作用：
继承Y中的所有public成员，从而可以访问数据
*/
#endif // PLAYER_H_INCLUDED
