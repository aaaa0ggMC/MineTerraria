#include "TerrarianNew.h"
#include <stdio.h>
#include <thread>
#include <algorithm>

using namespace std;
using namespace trnd;
using namespace game;


Chunk::Chunk(Pt2Di id,uint dimension){
    this->id = id;
    //outn(V_MAKE(this->id) << V_MAKE(ltbase()));
    this->dimension = dimension;;
    layers.clear();
    layers.insert(make_pair(DEF_BACKGOUND,Empty()));
    layers.insert(make_pair(1,Empty()));
    m_ref = 0;
}

Chunk::~Chunk(){
    for(auto &[id,v] : layers){
        for(auto & vv : *v){
            for(AbstractTile* t : vv){
                if(t)delete t;
            }
        }
        delete v;
    }
}

AbstractTile::AbstractTile(unsigned id):tile_id(id){

}

CDataDes* CH::QuickFindDes(HChunkDesc& f,vec<CDataDes>& d){
    for(CDataDes & dds : d){
        if(dds.desc == f)return &dds;
    }
    return NULL;
}

tile_set* Chunk::Empty(){
    tile_set * t = new tile_set(CHUNK_SIZE);
    int mx = 0,my = 0;
    for(tile_row & tr : *t){
        tr.resize(CHUNK_SIZE);
        for(AbstractTile * &tile: tr){
            if(mx == 8 && my==10)tile = NULL;
            else{
                tile = new AbstractTile(0);
                tile->Set(ltbase().x + mx,ltbase().y + my,false);
            }
            ++my;
        }
        ++mx;
        my = 0;
    }
    return t;
}

Chunk* CH::FindChunk(map<unsigned int,vector<Chunk*>>& m,unsigned int& d,Pt2Di& p){
    vector<Chunk*> * ck = NULL;
    for(auto & ele : m){
        if((ele.first) == d){
            ck = &ele.second;
            break;
        }
    }
    if(ck == NULL)return NULL;
    for(Chunk* c : (*ck)){
        if(c->id == p)return c;
    }
    return NULL;
}

vec<Pt2Di> CH::QuickBuildSurrId(Pt2Di cen,unsigned int len){
    vec<Pt2Di> r;
    for(int i = -(int)len;i <= (int)len;++i){
        for(int j = -(int)len;j <= (int)len;++j){
            r.push_back(cen + Pt2Di(i,j));
        }
    }
    return r;
}

AbstractTile* AbstractTile::Set(long x,long y,bool collision){
    this->x = x;
    this->y = y;
    this->collision = collision;
    return this;
}

sf::FloatRect AbstractTile::GenCollider(){
    return sf::FloatRect(x,y,1,1);
}
