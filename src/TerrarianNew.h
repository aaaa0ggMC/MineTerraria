#ifndef TERRARIANNEW_H_INCLUDED
#define TERRARIANNEW_H_INCLUDED
#include "kernel.h"
#include "./@terraria/random.h"
#include "helping.h"

using namespace std;
using namespace sf;

#define CHUNK_SIZE 32
#define BASE_TILSZ 128

#define DEF_BACKGOUND 0

#define Simple_Base(v) ((int)(((v)>0?(v)+CHUNK_SIZE/2:(v)-CHUNK_SIZE/2)/CHUNK_SIZE))


#define V_MAKE(V) " (" << V.x << "," << V.y << ") "
#define R_MAKE(V) " (" << V.left << "," << V.top << "," << V.width << "," << V.height << ") "


#define VSTR_MAKE(V) string(" (") + to_string(V.x) + "," + to_string(V.y) + ") "
#define RSTR_MAKE(V) string(" (") + to_string(V.left) + "," + to_string(V.top) + "," + to_string(V.width) + "," + to_string(V.height) + ") "

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
        tile_set* Empty();
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
        Vector2i ltbase(){return Vector2i(id.x*CHUNK_SIZE - CHUNK_SIZE/2,id.y*CHUNK_SIZE - CHUNK_SIZE/2);}
        IntRect getViewRect(){return IntRect(ltbase().x,ltbase().y,CHUNK_SIZE*BASE_TILSZ,CHUNK_SIZE*BASE_TILSZ);}
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
        return Pt2Di(Simple_Base(p.x),Simple_Base(p.y));
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
