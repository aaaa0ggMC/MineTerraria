#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED
#include "../TerrarianNew.h"
#include "ViewerGroup.h"
#include "player.h"
#define S_TILE_BASE "res/imgs/tiles/"
#include "requests.h"
#include "MapSave.h"
#include "../MusicController.h"
#include <rapidjson.h>
#include <unordered_map>
#include <CClock.h>

using namespace std;
using namespace sf;

namespace game{

    typedef void (*GenChunkData)(uint dimension_id,tile_set * background,tile_set * foreground,Chunk * chunk,unsigned int);

    struct GameManager{
        ///MSC
        MusicController * msc;
        ///Controls
        cck::Clock playedTime;

        ///Game Elements
        //Player
        Player * player;
        Sprite psp;
        //Map
        unordered_map<unsigned int,vec<Chunk*>> loaded;
        MapSave * save;

        alib::Document gc;

        long seed;

        ///Display
        ViewerGroup vg;
        FloatRect view;
        sf::View bindedView;
        Pt2Di from,end;
        bool showColliders;
        unordered_map<int,Texture*> tileTexs;
        unordered_map<int,Texture*> blkTexs;
        unordered_map<int,Sprite> templateSprites;
        unordered_map<int,Sprite> tblkSp;
        //Win w,Win h
        float w,h;

        ///Load
        void * wb;
        bool workerStop;
        vec<pthread_t> workers;
        //Requests
        RequestList rlist;
        vec<HChunkDesc> cCDes;//C Des
        vec<CDataDes> cCDDes;//Data Des

        unordered_map<uint,GenChunkData> trRegs;

        ///Game Design
        void ReadGameProperties(string data);

        ///Load permanent chunks
        void LoadPerm();
        ///Load & Unload a set of chunks
        void UnloadCDataDes(CDataDes&);
        CDataDes LoadCDataDes(HChunkDesc&);
        void UnloadChunks(vec<Chunk*>);
        vec<Chunk*> QuickBuildChunks(vec<Pt2Di> ids,unsigned int);

        ///Kernel Load&Unload Functions
        void UnloadChunk(Chunk*);
        Chunk * LoadChunk(unsigned int dimension,Pt2Di id);

        ///Player & Display
        void Bind(Player & pl,const View &);
        void UpdateDySingle();
        void UpdateView();
        void Paint(RenderTarget&);
        static Texture * loadTex(string path);

        ///About game starting & ending
        void EndupGame();
        void ResumeGame();
        int appendTexture(int,string path);
        int appendBlockTexture(int,string path);

        ///Workers to work
        void StartWorkerThread(unsigned int c);
        static void * workerFn(void *);

        void SetPlayerSprite(Sprite sp);

        void BindMusicController(MusicController * msc){this->msc = msc;}

        ///Events
        void OnPress(Event & e);

        ///Generatings
        void GenChunk(Chunk *);
        void SaveChunk(Chunk *);

        ///Basic Functions
        GameManager(long seed = 0);
        ~GameManager();
    };

    struct WorkerBinded{
        RequestList * rl;
        GameManager * gm;
    };
}

#endif // MANAGER_H_INCLUDED
