#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "../TerrarianNew.h"

using namespace std;
using namespace sf;

namespace game{
    class Player{
    public:
        ///是图格地址，而不是屏幕地址
        Vector2f position;
        vector<Chunk*> rChunks;//dynamic loading chunks
        unsigned int rSize;//Streching size,0 means 1*1,1 means 3*3
        unsigned int dimension;
        unsigned int uuid_local;
        Pt2D<float> texSz;
        Pt2D<float> cptg;//Collision Percentage
        Pt2D<float> veclocity;

        Player & setPosition(Vector2f p,uint ldimension){position = p;dimension = ldimension;return *this;}
        Player & setPosition(float x,float y,uint di){position.x = x;position.y = y;dimension = di;return *this;}
        Player & setCollisionPercen(Vector2f v){cptg = v;return *this;}
        Player & setCollisionPercen(float x,float y){return setCollisionPercen({x,y});}
        Player & setBasicInformation(unsigned int uuid_local){this->uuid_local = uuid_local;return *this;}
        Player & setDyChunkInfo(uint rSize = 1){this->rSize = rSize;return *this;}

        void Move(float x,float y){position.x += x;position.y += y;}
        FloatRect GenMoveCollider(float x,float y);
        FloatRect GetRect();
        Pt2D<float> GetFormedScale(float baseSize);
    };
}

#endif // PLAYER_H_INCLUDED
