#include "Registry.h"
using namespace game;
using namespace std;

bool Register::RegisterTiles(vector<Register::RTile> tiles){
    bool rt = true;
    for(auto& [uuid,path] : tiles){
        if(g->appendTexture(uuid,path))rt = false;
    }
    return rt;
}


bool Register::RegisterTilesStuck(vector<Register::RTile> tiles){
    for(auto& [uuid,path] : tiles){
        if(g->appendTexture(uuid,path))return false;
    }
    return true;
}

Register::Register(GameManager& gm):g(&gm){}

bool Register::RegisterTile(Register::RTile t){
    return !g->appendTexture(t.a,t.b);
}

bool Register::RegisterBlocks(vector<Register::RTile> tiles){
    bool rt = true;
    for(auto& [uuid,path] : tiles){
        if(g->appendBlockTexture(uuid,path))rt = false;
    }
    return rt;
}


bool Register::RegisterBlocksStuck(vector<Register::RTile> tiles){
    for(auto& [uuid,path] : tiles){
        if(g->appendBlockTexture(uuid,path))return false;
    }
    return true;
}

bool Register::RegisterBlock(Register::RTile t){
    return !g->appendBlockTexture(t.a,t.b);
}

