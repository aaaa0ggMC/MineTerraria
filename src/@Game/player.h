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
        void Move(float x,float y){position.x += x;position.y += y;}
    };
}

#endif // PLAYER_H_INCLUDED
