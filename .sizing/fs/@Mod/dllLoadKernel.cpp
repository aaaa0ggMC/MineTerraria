#include "dllLoadKernel.h"

using namespace std;

vector<Mod> GetAllAvaliableMods(vector<string> & paths){
    HMODULE mod = NULL;
    vector<Mod> ret;
    for(string & a : paths){
        mod = LoadLibrary(a.c_str());
        if(mod){
            ret.push_back(Mod(mod,a));
        }
    }
    return ret;
}

void UnistallLibraries(vector<HMODULE> & mods){
    for_each(mods.begin(),mods.end(),[&](HMODULE & in){FreeLibrary(in);});
}
