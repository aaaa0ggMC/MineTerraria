#ifndef DLLLOADKERNEL_H_INCLUDED
#define DLLLOADKERNEL_H_INCLUDED
#define DLL_KERNEL_ONLY_FOR_DLL
#include "dllKernel.h"
#include "../kernel.h"

using namespace std;

struct Mod{
    HMODULE module;
    string path;
    ModInfo info;
    _GetModInfo GMI;
    Mod(HMODULE md,string path){module = md;this->path = path;}
};

vector<Mod> GetAllAvaliableMods(vector<string>&);
void UnistallLibraries(vector<HMODULE> &);


struct ModsHelper{
    vector<Mod> mods;
};

#endif // DLLLOADKERNEL_H_INCLUDED
