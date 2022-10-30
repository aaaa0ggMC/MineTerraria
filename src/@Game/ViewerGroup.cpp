#include "ViewerGroup.h"

using namespace std;
using namespace game;

void ViewerGroup::push(Chunk * c){unseri.push_back(c);}
void ViewerGroup::clear(){unseri.clear();}
AbstractTile* ViewerGroup::operator()(Pt2Di ptAb,int layer){
    //cout << ptAb.x << " " << ptAb.y << endl;
    Pt2Di rela = ptAb - Vmaps[0][0]->ltbase();
    Pt2Di inrela = rela / CHUNK_SIZE,rest = rela % CHUNK_SIZE;
    if(inrela.x >= (int)Vmaps.size())return NULL;
    else if((int)Vmaps[inrela.x].size() <= inrela.y)return NULL;
    //cout << "Viewer:" << rela.x << " " << rela.y << " :" << inrela.x << " " << inrela.y << " :" << rest.x << " " << rest.y << endl;
    //cout << (*(Vmaps[inrela.x][inrela.y]->layers[layer]))[rest.x][rest.y] << endl;
    return (*(Vmaps[inrela.x][inrela.y]->layers[layer]))[rest.x][rest.y];
}
void ViewerGroup::form(){
    if(!unseri.size())return;
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
                tmp.clear();
            }
        }
        tmp.push_back(unseri[i]);
    }
    Vmaps.push_back(tmp);
}
