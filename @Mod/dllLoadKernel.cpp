#include "dllLoadKernel.h"

using namespace std;

vector<HMODULE> GetAllAvaliableMods(vector<string> & paths){
    HMODULE mod = NULL;
    vector<HMODULE> ret;
    for(string & a : paths){
        mod = LoadLibrary(a.c_str());
        if(mod){
            ret.push_back(mod);
        }
    }
}

void UnistallLibraries(vector<HMODULE> & mods){
    for_each(mods.begin(),mods.end(),[&](HMODULE & in){FreeLibrary(in);});
}
