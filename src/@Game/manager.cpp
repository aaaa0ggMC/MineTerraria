#include "manager.h"

using namespace std;
using namespace sf;
using namespace game;

void GameManager::LoadPerm(){
    vec<CDataDes> tmp_c;
    for(HChunkDesc & cd : this->cCDes){
        CDataDes * c = CH::QuickFindDes(cd,cCDDes);
        ////这里引入了异常
        //if(!c)throw "GameCrashed";
        if(c)UnloadCDataDes(*c);
        tmp_c.push_back(LoadCDataDes(cd));
    }
    cCDDes = tmp_c;
}

Chunk * GameManager::LoadChunk(unsigned int dimension,Pt2Di id){
    Chunk * c = CH::FindChunk(loaded,dimension,id);
    if(!c){
        c = new Chunk(id,dimension);
        loaded[dimension].push_back(c);
    }
    c->addRef();
    return c;
}

void GameManager::UnloadChunk(Chunk* c){
    c->delRef();
    if(c->needDestory()){
        auto cc = find(loaded[c->dimension].begin(),loaded[c->dimension].end(),c);
        if(cc != loaded[c->dimension].end()){//Means the chunk is maybe a ghost chunk
            loaded[c->dimension].erase(cc);
        }
        delete c;
        cout << "Unloaded:" << c->id.x << " " << c->id.y << endl;
    }
}


void GameManager::UnloadCDataDes(CDataDes& w){
    for(Chunk* c : w.chunks){
        UnloadChunk(c);
    }
}

CDataDes GameManager::LoadCDataDes(HChunkDesc& w){
    CDataDes ret;
    ret.chunks = QuickBuildChunks(CH::QuickBuildSurrId(w.center,w.size),w.dimension);
    ret.desc = w;
    return ret;
}

vec<Chunk*> GameManager::QuickBuildChunks(vec<Pt2Di> ids,unsigned int b_dimension){
    vec<Chunk*> c;
    for(Pt2Di & p : ids){
        c.push_back(LoadChunk(b_dimension,p));
    }
    return c;
}

void GameManager::BindPlayer(Player * pl){
    player = pl;
}

void GameManager::UpdateDySingle(){
    Player&p = *player;
    vector<Pt2Di> chunkIds = CH::QuickBuildSurrId(ChunkId(p.position),p.rSize);
    vector<Chunk*> tmp_c = this->QuickBuildChunks(chunkIds,p.dimension);
    UnloadChunks(p.rChunks);
    p.rChunks = tmp_c;
}

void GameManager::UnloadChunks(vec<Chunk*> c){
    for(Chunk * d : c){
        UnloadChunk(d);
    }
}

void GameManager::UpdateView(){
    Player & p = *player;
    view.left = p.position.x - w/2;
    view.top = p.position.y - h/2;
    view.width = w;
    view.height = h;
    vg.clear();
    for(Chunk * c : p.rChunks){
        if(view.intersects(toARect<int,float>(c->getRect())))vg.push(c);
    }
    vg.form();
    Pt2Di ipp = toInt(p.position);
    Pt2Di halo(w/2 / BASE_TILSZ +1 ,h/2 / BASE_TILSZ + 1);
    from = ipp - halo;
    end = ipp + halo;
}

void GameManager::Paint(RenderTarget& t){
    Pt2D<float> start = (toFloat(toInt(player->position)) - player->position) * (float)BASE_TILSZ;
    Pt2Di pf = from;
    AbstractTile * b;
    for(;pf.x <= end.x;++pf.x){
        for(;pf.y <= end.y;++pf.y){
            b = vg(pf,DEF_BACKGOUND);
            if(b)t.draw(CH::buildSprite(tileTexs.at(b->tile_id),start + toFloat(BASE_TILSZ * (pf - from)) ));
        }
        pf.y = from.y;
    }
}

Texture * GameManager::loadTex(string path){
    Texture * t = new Texture();
    if(!t)return NULL;
    if(!t->loadFromFile(path)){
        delete t;
        return NULL;
    }
    return t;
}

int GameManager::appendTexture(int uuid,string path){
    Texture * t = loadTex(path);
    if(t){
        tileTexs.insert(make_pair(uuid,t));
        return 0;
    }
    return -1;
}
