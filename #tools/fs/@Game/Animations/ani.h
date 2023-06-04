#ifndef ANI_H_INCLUDED
#define ANI_H_INCLUDED

#include <string>
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <unordered_map>

using namespace std;
using namespace sf;

namespace game{
    class Animation{
        unordered_map<string,vector<Texture*>> dataPool;
        Animation();
    };
}

#endif // ANI_H_INCLUDED
