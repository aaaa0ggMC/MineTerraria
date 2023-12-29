#ifndef VIEWERGROUP_H_INCLUDED
#define VIEWERGROUP_H_INCLUDED
#include "../TerrarianNew.h"

using namespace std;

namespace game{

    struct ViewerGroup{
        vector<Chunk*> unseri;
        vector<vector<Chunk*>> Vmaps;
        void push(Chunk*);
        void form();
        void clear();
        //Absolute position
        AbstractTile* operator()(Pt2Di ptAb,int);
    };
}

#endif // VIEWERGROUP_H_INCLUDED
