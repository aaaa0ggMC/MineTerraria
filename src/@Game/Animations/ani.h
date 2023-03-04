#ifndef ANI_H_INCLUDED
#define ANI_H_INCLUDED

#include "../../kernel.h"

using namespace std;
using namespace sf;

namespace game{
    class Animation{
        map<string,vector<Texture*>> dataPool;
        Animation();
    };
}

#endif // ANI_H_INCLUDED
