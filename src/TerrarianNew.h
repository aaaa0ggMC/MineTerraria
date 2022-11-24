#ifndef TERRARIANNEW_H_INCLUDED
#define TERRARIANNEW_H_INCLUDED
#include "kernel.h"
#include "./@terraria/random.h"
#include "helping.h"

using namespace std;
using namespace sf;

#define CHUNK_SIZE 32
#define BASE_TILSZ 64

#define DEF_BACKGOUND 0

namespace game{

    template<class T> using Pt2D = Vector2<T>;
    using Pt2Di = Vector2i;
    using uint = unsigned int;
    template<class T> using vec = vector<T>;

    template<class T> Pt2D<T> operator%(Pt2D<T> a,T sc){
        return Pt2D<T>(a.x % sc,a.y % sc);
    }

    template<class T> Pt2D<int> toInt(Pt2D<T> a){
        return Pt2D<int>((int)a.x,(int)a.y);
    }

    template<class T> Pt2D<float> toFloat(Pt2D<T> a){
        return Pt2D<float>((float)a.x,(float)a.y);
    }


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
                    tile = new AbstractTile(rand()%2);
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
        ///返回左上角空间中的绝对地址，而非id
        Vector2i ltbase(){return Vector2i(id.x,id.y) * 32;}
        IntRect getRect(){return IntRect(CHUNK_SIZE*ltbase().x,CHUNK_SIZE*ltbase().y,
                                         CHUNK_SIZE*BASE_TILSZ,CHUNK_SIZE*BASE_TILSZ);}
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
        static Sprite buildSprite(Sprite & t,Pt2D<float> pos){
            Sprite sp = t;
            sp.setPosition(pos);
            return sp;
        }
    };

    using CH = ChunkHelper;

    template<class T> Pt2Di ChunkId(Pt2D<T> p){
        return Pt2Di(p.x/CHUNK_SIZE,p.y/CHUNK_SIZE);
    }

    template<class T,class V> Rect<V> toARect(Rect<T> in){
        Rect<V> ret;
        ret.left = (V)in.left;
        ret.top = (V)in.top;
        ret.width = (V)in.width;
        ret.height = (V)in.height;
        return ret;
    }
}

#endif // TERRARIANNEW_H_INCLUDED
