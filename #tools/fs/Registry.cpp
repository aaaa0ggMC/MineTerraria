#include "Registry.h"
using namespace game;
using namespace std;

bool Register::RegisterTiles(vector<Register::RTile> tiles){
    bool rt = true;
    for(auto& [uuid,path] : tiles){
        if(gm->appendTexture(uuid,path))rt = false;
    }
    return rt;
}


bool Register::RegisterTilesStuck(vector<Register::RTile> tiles){
    for(auto& [uuid,path] : tiles){
        if(gm->appendTexture(uuid,path))return false;
    }
    return true;
}

Register::Register(GameManager& gm){
    g = &gm;
}
