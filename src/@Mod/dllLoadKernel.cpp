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


EventFunc GetFunctionFromEvent(MEvent & e,string key){
    for(auto k : e.funcs){
        if(!k.first.compare(key)){
            return k.second;
        }
    }
    return NULL;
}

EventRet CallEventFunction(EventFunc fn,EventArgs a){
    if(fn){
        return fn(a);
    }else return ERET_VOID;
}

EventRet CallFunctionFromEvent(MEvent & e,string key,EventArgs a){
    EventFunc fn = GetFunctionFromEvent(e,key);
    return CallEventFunction(fn,a);
}
