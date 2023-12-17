#include "main.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <stdlib.h>
#include <spdlog.h>
#include <Translator.h>

using namespace std;

DLL_BASIC

ModInfo mi("SB_CNM2","测试傻逼的模组","YSLLLLLLL","还能有什么简介",Version("NMSL Alpha",114,514,"1919810"));
alib::LogFactory mlg("测试傻逼的模组",true);


ModInfo MOD_API GetModInfo(BasicInfo bi){
    mlg.critical("我的天哪！");
    mi.DoVaild();
    return mi;
}

InitInfo MOD_API InitializeMod(InitInfoOut o){
    InitInfo info;
    (*o.titleString).clear();
    (*o.titleString).push_back("草泥马！");
    (*o.titleString).push_back("你有神经病十八！");
    (*o.titleString).push_back("哦买噶！你是不是要死了");
    MessageBox(NULL,"你妈死了","...",MB_OK | MB_TOPMOST | MB_ICONERROR);
    return info;
}
