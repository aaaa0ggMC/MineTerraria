#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "../TerrarianNew.h"

using namespace std;

namespace game{
    class Player{
    public:
        vector<Chunk*> rChunks;//dynamic loading chunks
        unsigned int rSize;//Streching size,0 means 1*1,1 means 3*3
        unsigned int dimension;
    };
}

#endif // PLAYER_H_INCLUDED
