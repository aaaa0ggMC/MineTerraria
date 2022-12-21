#ifndef REGISTRY_H_INCLUDED
#define REGISTRY_H_INCLUDED
#include "../kernel.h"
#include "manager.h"

namespace game{
    struct Register{
        struct RTile{
            int a;string b;
        };
        Register(GameManager& gm);
        bool RegisterTiles(vector<RTile> tiles);
        bool RegisterBlocks(vector<RTile> tiles);
        bool RegisterTile(RTile tiles);
        bool RegisterBlock(RTile tiles);
        bool RegisterTilesStuck(vector<RTile> tiles);
        bool RegisterBlocksStuck(vector<RTile> tiles);
        GameManager * g;
    };
}

#endif // REGISTRY_H_INCLUDED
