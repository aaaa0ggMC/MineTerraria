#include "ViewerGroup.h"

using namespace std;
using namespace game;

void ViewerGroup::push(Chunk * c){unseri.push_back(c);}
void ViewerGroup::clear(){unseri.clear();}
AbstractTile* ViewerGroup::operator()(Pt2Di ptAb,int layer){
    if(Vmaps.size() <=0)return NULL;
    else if(Vmaps[0].size() <= 0)return NULL;
    ///问题：这里的xy 搞反了！！！！
    ///因此 Vmaps访问时x y 倒置

    Pt2Di rela = ptAb - Vmaps[0][0]->ltbase();
    Pt2Di inrela = rela / CHUNK_SIZE,rest = rela % CHUNK_SIZE;
    if(inrela.y < 0 || inrela.y >= (int)Vmaps.size())return NULL;
    else if(inrela.x < 0 || (int)Vmaps[inrela.y].size() <= inrela.x)return NULL;
    return (*(Vmaps[inrela.y][inrela.x]->layers[layer]))[rest.x][rest.y];
}
void ViewerGroup::form(){
    if(!unseri.size())return;
    Vmaps.clear();
    std::sort(unseri.begin(),unseri.end(),[&](Chunk * a,Chunk * b){
        if(a->id.y > b->id.y)return false;
        else if(a->id.y == b->id.y && a->id.x > b->id.x)return false;
        else return true;
    });
    vec<Chunk*> tmp;
    for(unsigned int i = 0;i < unseri.size();++i){
        if(i != 0){
            if(unseri[i-1]->id.y != unseri[i]->id.y){
                Vmaps.push_back(tmp);
                tmp = {};
            }
        }
        tmp.push_back(unseri[i]);
    }
    Vmaps.push_back(tmp);
}
