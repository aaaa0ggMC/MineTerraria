#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED
#include "../TerrarianNew.h"
#include "ViewerGroup.h"
#include "player.h"
#define S_TILE_BASE "res/imgs/tiles/"

using namespace std;
using namespace sf;

namespace game{
    struct GameManager{
        Player * player;
        ViewerGroup vg;
        FloatRect view;
        Pt2Di from,end;
        map<int,Texture*> tileTexs;
        ///Win w,Win h
        float w,h;
        map<unsigned int,vec<Chunk*>> loaded;
        vec<HChunkDesc> cCDes;//C Des
        vec<CDataDes> cCDDes;//Data Des
        void LoadPerm();
        void UnloadCDataDes(CDataDes&);
        CDataDes LoadCDataDes(HChunkDesc&);
        Chunk * LoadChunk(unsigned int dimension,Pt2Di id);
        void UnloadChunk(Chunk*);
        void UnloadChunks(vec<Chunk*>);
        vec<Chunk*> QuickBuildChunks(vec<Pt2Di> ids,unsigned int);
        void BindPlayer(Player * pl);
        void UpdateDySingle();
        void UpdateView();
        void Paint(RenderTarget&);
        int appendTexture(int,string path);
        static Texture * loadTex(string path);
        ~GameManager(){
            for(auto & a : tileTexs){
                delete (a.second);
            }
        }
    };
}

#endif // MANAGER_H_INCLUDED
