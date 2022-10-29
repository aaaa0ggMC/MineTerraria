#ifndef TERRARIANNEW_H_INCLUDED
#define TERRARIANNEW_H_INCLUDED
#include "kernel.h"
#include "./@terraria/random.h"
#include "helping.h"

using namespace std;
using namespace sf;

#define CHUNK_SIZE 32

#define DEF_BACKGOUND 0

namespace game{

    template<class T> using Pt2D = Vector2<T>;
    using Pt2Di = Vector2i;
    using uint = unsigned int;
    template<class T> using vec = vector<T>;

    //just a tile
    struct AbstractTile{
        unsigned int tile_id;
        AbstractTile(unsigned id):tile_id(id){}
    };

    using tile_set = vec<vec<AbstractTile*>>;
    using tile_row = vec<AbstractTile*>;

    //the chunk
    struct Chunk{
        map<int,tile_set*> layers;
        Pt2Di id;
        uint dimension;
        tile_set* Empty(){
            tile_set * t = new tile_set(CHUNK_SIZE);
            for(tile_row & tr : *t){
                tr.resize(CHUNK_SIZE);
                for(AbstractTile * &tile: tr){
                    tile = new AbstractTile(0);
                }
            }
            return t;
        }
        Chunk(Pt2Di id,uint dimension){
            this->id = id;
            this->dimension = dimension;;
            layers.insert(make_pair(DEF_BACKGOUND,Empty()));
            m_ref = 0;
        }
        void addRef(){++m_ref;}
        void delRef(){if(m_ref > 0)--m_ref;}
        unsigned int getRef(){return m_ref;}
        bool needDestory(){return m_ref==0;}
    private:
        unsigned int m_ref;
    };

    //Highly load chunk descriptor
    struct HChunkDesc{
        Pt2Di center;
        uint size;//See player.cpp Player::rSize
        uint dimension;
        bool operator==(HChunkDesc& a){
            if(a.dimension == this->dimension && a.center == this->center && a.size == this->size)return true;
            return false;
        }
    };
    //Chunk Data Descriptor
    struct CDataDes{
        vector<Chunk*> chunks;
        HChunkDesc desc;
    };

    struct ChunkHelper{
        static CDataDes* QuickFindDes(HChunkDesc&,vec<CDataDes>&);
        static Chunk* FindChunk(map<unsigned int,vector<Chunk*>>&,unsigned int&,Pt2Di&);
        static vec<Pt2Di> QuickBuildSurrId(Pt2Di cen,unsigned int len);
    };

    using CH = ChunkHelper;
}

#endif // TERRARIANNEW_H_INCLUDED
