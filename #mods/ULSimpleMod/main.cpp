#include "main.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <stdlib.h>

using namespace std;

DLL_BASIC

ModInfo mi("MOD_SIMPLE_RCFFST-WYDSA-278382-DJSHHD-72","SimpleMod","aaaa0ggmc","None",Version("alpha"));

int i = 0;
string v = "";
void fn(void){
    cout << i << " " << v << endl;
}


ModInfo MOD_API GetModInfo(BasicInfo bi){
    plg(mi,"Selected Language Id:" + to_string(bi.Language) + "\n");
    plg(mi,"Hello World!\n");
    mi.dllKernelVersion = MOD_VER;
    mi.test.fn = fn;
    mi.test.iv = &i;
    mi.test.vv = &v;
    mi.DoVaild();
    return mi;
}

InitInfo MOD_API InitializeMod(InitInfoOut o){
    InitInfo info;
    info.hasLog = true;
    *o.titleString = {
        "你好世界",
        "现在ModEngine版本为 " + to_string(MOD_VER)
    };
    return info;
}
