#ifndef GEN_H_INCLUDED
#define GEN_H_INCLUDED
#include "../../TerrarianNew.h"
#include "../random.h"
#include "../../@Game/IDs.h"
#include <math.h>

using namespace game;

///在这里编写主世界区块类型等等的生成
inline unsigned int genType(int x,int y,int seed){
    trnd::Random r((int)(x*x - y*y - 10*sin(x) - 10*cos(y)+ seed));
    return r.genUint()+1;
}

#define OV_CHUNK_GRASS 1
#define OV_CHUNK_SAND 2

void OverWorldGen(uint dimension_id,tile_set * t,tile_set * t1,Chunk * c,unsigned int seed){
    switch(genType(c->id.x,c->id.y,seed)%3){
        case 0:
        case OV_CHUNK_GRASS:{
            trnd::Random r(seed);
            for(tile_row & tr : *t){
                for(AbstractTile * &tile: tr){
                    if(tile){
                        if(r.genUint(10000) > 8500)tile->tile_id = TILE_SAND;
                        else tile->tile_id = TILE_GRASS;
                    }
                }
            }
            for(tile_row & tr : *t1){
                for(AbstractTile * &tile: tr){
                    if(tile){
                        if(r.genUint(10000) > 9000){
                            tile->tile_id = 1;
                            tile->collision = true;
                        }else {
                            delete tile;
                            tile = NULL;
                        }
                    }
                }
            }
            break;
        }
        case OV_CHUNK_SAND:{
            trnd::Random r(seed);
            for(tile_row & tr : *t){
                for(AbstractTile * &tile: tr){
                    if(tile){
                        if(r.genUint(10000) > 9000)tile->tile_id = TILE_GRASS;
                        else tile->tile_id = TILE_SAND;
                    }
                }
            }
            for(tile_row & tr : *t1){
                for(AbstractTile * &tile: tr){
                    if(tile){
                        if(r.genUint(10000) > 9400){
                            tile->tile_id = 1;
                            tile->collision = true;
                        }else {
                            delete tile;
                            tile = NULL;
                        }
                    }
                }
            }
            break;
        }
    }
}

#endif // GEN_H_INCLUDED
