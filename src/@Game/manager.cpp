#include "manager.h"

using namespace std;
using namespace sf;
using namespace game;

GameManager::GameManager(){
    wb = (void*)(new WorkerBinded());
    ((WorkerBinded*)(wb))->rl = &rlist;
    ((WorkerBinded*)(wb))->gm = this;
}

GameManager::~GameManager(){
    for(auto & a : tileTexs){
        delete (a.second);
    }
    for(auto b : workers){
        pthread_cancel(b);
    }
}

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
        MAKE_REQ(LoadChunk,targetChunk,c);
        loaded[dimension].push_back(c);
    }
    c->addRef();
    return c;
}

void GameManager::UnloadChunk(Chunk* c){
    c->delRef();
    if(c->needDestory()){
        vec<Chunk*> csgo;
        for(Chunk * cc: loaded[c->dimension]){
            if(cc != c)csgo.push_back(cc);
        }
        loaded[c->dimension] = csgo;
        //cout << "Unloading " << c << endl;
        delete c;
    }
}


void GameManager::UnloadCDataDes(CDataDes& w){
    for(Chunk* c : w.chunks){
        MAKE_REQ(UnloadChunk,targetChunk,c);
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

void GameManager::Bind(Player & pl,const View & view){
    player = &pl;
    bindedView = view;
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
        MAKE_REQ(UnloadChunk,targetChunk,d);
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
        vg.push(c);
    }
    vg.form();
    Pt2Di ipp = toInt(p.position);
    Pt2Di halo(w/2 / (float)(BASE_TILSZ) ,h/2 / (float)(BASE_TILSZ));
    from = ipp - halo - Pt2Di(2,2);
    end = ipp + halo + Pt2Di(2,2);
}

void GameManager::Paint(RenderTarget& t){
    bindedView.setCenter(player->position * (float)(BASE_TILSZ));
    Pt2Di pf = from;
    AbstractTile * b;
    t.setView(bindedView);
    RectangleShape rs({BASE_TILSZ,BASE_TILSZ});
    rs.setOutlineColor(Color::Black);
    rs.setOutlineThickness(4.0);
    rs.setFillColor(Color(0,0,0,0));
    for(;pf.x <= end.x;++pf.x){
        for(;pf.y <= end.y+1;++pf.y){
            b = vg(pf,DEF_BACKGOUND);
            if(b)t.draw(CH::buildSprite(templateSprites[b->tile_id],{(float)(pf.x * BASE_TILSZ),(float)(pf.y * BASE_TILSZ)}));
            b = vg(pf,1);
            if(b){
                t.draw(CH::buildSprite(tblkSp[b->tile_id],{(float)(pf.x * BASE_TILSZ),(float)(pf.y * BASE_TILSZ)}));
                if(showColliders){
                    rs.setPosition(b->x * BASE_TILSZ,b->y * BASE_TILSZ);
                    t.draw(rs);
                }
            }
        }
        pf.y = from.y;
    }
    if(showColliders){
        rs.setPosition({player->GetRect().left * BASE_TILSZ,player->GetRect().top * BASE_TILSZ});
        rs.setSize({player->GetRect().width * BASE_TILSZ,player->GetRect().height * BASE_TILSZ});
        t.draw(rs);
    }
    t.setView(t.getDefaultView());
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


static inline Sprite buildTemplateSprite(Texture * t){
    Sprite sp(*t);
    sp.setScale((float)BASE_TILSZ / t->getSize().x,(float)BASE_TILSZ / t->getSize().y);
    return sp;
}

int GameManager::appendTexture(int uuid,string path){
    Texture * t = loadTex(path);
    if(t){
        tileTexs.insert(make_pair(uuid,t));
        templateSprites.insert(make_pair(uuid,buildTemplateSprite(t)));
        return 0;
    }
    return -1;
}

int GameManager::appendBlockTexture(int uuid,string path){
    Texture * t = loadTex(path);
    if(t){
        blkTexs.insert(make_pair(uuid,t));
        tblkSp.insert(make_pair(uuid,buildTemplateSprite(t)));
        return 0;
    }
    return -1;
}

void GameManager::EndupGame(){
    workerStop = true;
    ///Release Chunk Data
    player->rChunks.clear();
    for(auto & c : loaded){
        for(Chunk * cs : c.second){
            SaveChunk(cs);
            UnloadChunk(cs);
        }
    }
    while(!rlist.empty())rlist.pop();
}

void GameManager::ResumeGame(){
    workerStop = false;
}

#define WK_RESTTIME 10
void * GameManager::workerFn(void * d){
    WorkerBinded & bd = *((WorkerBinded*)d);
    RequestList & rl = *(bd.rl);
    while(true){
        if(bd.gm->workerStop){
            Sleep(WK_RESTTIME * 15);
            continue;
        }
        Request r = rl.pop();
        if(r.qt == Request::NoRequest){
            Sleep(WK_RESTTIME);
            continue;
        }else if(r.qt == Request::LoadChunk){
            bd.gm->GenChunk(r.targetChunk);
        }else if(r.qt == Request::UnloadChunk){
            bd.gm->SaveChunk(r.targetChunk);
            bd.gm->UnloadChunk(r.targetChunk);
        }
    }
    return NULL;
}

void GameManager::SaveChunk(Chunk * c){
    //保存区块数据
}

void GameManager::GenChunk(Chunk* c){
    tile_set * t = c->layers[DEF_BACKGOUND];
    for(tile_row & tr : *t){
        for(AbstractTile * &tile: tr){
            if(tile)tile->tile_id = (int)abs(c->id.x)%2;
        }
    }
    delete ((*t)[15][15]);
    delete ((*t)[0][0]);
    (*t)[15][15] = NULL;
    (*t)[0][0] = NULL;
    t = c->layers[1];
    int mod = 0;
    for(tile_row & tr : *t){
        for(AbstractTile * &tile: tr){
            if(tile){
                if(mod%3 == 2 && tile->y != 0){
                    tile->tile_id = 0;
                    tile->collision = true;
                }else {
                    delete tile;
                    tile = NULL;
                }
            }
        }
        ++mod;
    }
}

void GameManager::StartWorkerThread(unsigned int c){
    loop(c){
        pthread_t p = 0;
        pthread_create(&p,NULL,GameManager::workerFn,wb);
        if(p){
            workers.push_back(p);
            pthread_detach(p);
        }
    }
}
