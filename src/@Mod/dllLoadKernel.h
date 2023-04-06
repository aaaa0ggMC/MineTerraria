#ifndef DLLLOADKERNEL_H_INCLUDED
#define DLLLOADKERNEL_H_INCLUDED
#define DLL_KERNEL_ONLY_FOR_DLL
#include "dllKernel.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Mod{
    HMODULE module;
    string path;
    ModInfo info;
    InitInfo initInfo;
    _GetModInfo GMI;
    _InitMod IM;
    Mod(HMODULE md,string path){module = md;this->path = path;}
};

vector<Mod> GetAllAvaliableMods(vector<string>&);
void UnistallLibraries(vector<HMODULE> &);
EventFunc GetFunctionFromEvent(MEvent & e,string key);
EventRet CallFunctionFromEvent(MEvent & e,string key,EventArgs a);
EventRet CallEventFunction(EventFunc fn,EventArgs a);

struct ModsHelper{
    vector<Mod> mods;
};

#define SafeCall(fn,...) if(fn){fn(__VA_ARGS__);}

#endif // DLLLOADKERNEL_H_INCLUDED
