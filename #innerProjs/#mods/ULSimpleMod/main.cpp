#include "main.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <stdlib.h>
#include <spdlog.h>
#include <Translator.h>

using namespace std;

DLL_BASIC

ModInfo mi("SB_CNM","SimpleMod","aaaa0ggmc","None",Version("alpha"));
alib::LogFactory mlg("TestMod",true);
alib::Translator* tr = alib::Translator::get();

int i = 0;
string v = "";
void fn(void){
    cout << i << " " << v << endl;
}


ModInfo MOD_API GetModInfo(BasicInfo bi){
    plg(mi,"Selected Language Id:" + to_string(bi.Language) + "\n");
    plg(mi,"Hello World!\n");
    plg(mi,"Hi everybody!This new content is added on Sun June 4 2023 3:43pm!\n");
    mi.dllKernelVersion = MOD_VER;
    mi.test.fn = fn;
    mi.test.iv = &i;
    mi.test.vv = &v;
    mi.DoVaild();
    return mi;
}

InitInfo MOD_API InitializeMod(InitInfoOut o){
    InitInfo info;
    (*o.titleString).push_back("你好世界");
    (*o.titleString).push_back("现在ModEngine版本为 " + to_string(MOD_VER));
    plg(info,"卧槽，这是一段再InitializeMod执行的Log!!!!!");
    mlg.warn("你好世界！！");
    info.hasLog = true;
    return info;
}
