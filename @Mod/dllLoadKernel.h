#ifndef DLLLOADKERNEL_H_INCLUDED
#define DLLLOADKERNEL_H_INCLUDED
#define DLL_KERNEL_ONLY_FOR_DLL
#include "dllKernel.h"
#include "../kernel.h"

using namespace std;

vector<HMODULE> GetAllAvaliableMods(const vector<string>&);
void UnistallLibraries(const vector<HMODULE> &);

#endif // DLLLOADKERNEL_H_INCLUDED
