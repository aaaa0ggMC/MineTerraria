#include "main.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <stdlib.h>
#include <spdlog.h>
#include <Translator.h>

using namespace std;

DLL_BASIC

ModInfo mi("SB_CNM2","����ɵ�Ƶ�ģ��","YSLLLLLLL","������ʲô���",Version("NMSL Alpha",114,514,"1919810"));
alib::LogFactory mlg("����ɵ�Ƶ�ģ��",true);


ModInfo MOD_API GetModInfo(BasicInfo bi){
    mlg.critical("�ҵ����ģ�");
    mi.DoVaild();
    return mi;
}

InitInfo MOD_API InitializeMod(InitInfoOut o){
    InitInfo info;
    (*o.titleString).clear();
    (*o.titleString).push_back("������");
    (*o.titleString).push_back("�����񾭲�ʮ�ˣ�");
    (*o.titleString).push_back("Ŷ��������ǲ���Ҫ����");
    MessageBox(NULL,"��������","...",MB_OK | MB_TOPMOST | MB_ICONERROR);
    return info;
}
