#ifndef TRANSLATOR_H_INCLUDED
#define TRANSLATOR_H_INCLUDED

#include "kernel.h"
#include "@Game/KERNEL_INFO.h"
#include "MultiEnString.h"

#define VERIFY_TOKEN "Language"
#define ACCESS_TOKEN "Access"

using namespace std;

#define MultiTranslate(obj,oid,odv,enc,...) [&](Translator o,string id,string dv){bool rep = false;if(o.currentTranslates->find(id) == o.currentTranslates->end())rep = true;\
    string r="";char * buf = new char[TEXT_MAX_SIZE];memset(buf,0,sizeof(char)*TEXT_MAX_SIZE);\
    sprintf_s(buf,sizeof(char) * (TEXT_MAX_SIZE-1),rep?(dv.c_str()):((*o.currentTranslates)[id].c_str()),__VA_ARGS__);\
    buf[TEXT_MAX_SIZE-1] = '\0';\
    r = buf;\
    delete [] buf;return MultiEnString(r,MultiEnString::Utf8);}(obj,oid,odv)

///文字多语言支持
class Translator{
public:
    ///返回得到的语言数量
    int LoadTranslateFiles(string path);
    map<string,string> *currentTranslates;
    map<string,string> *fstTl;
    int LoadTranslate(string lan_id,string defId);
    map<string,map<string,string>> summTrans;
    MultiEnString Translate(string id,string def = "",MultiEnString::EncType enc = MultiEnString::Utf8);
    void SetFisrtID(string defId);
    Translator():fstTl(NULL),currentTranslates(NULL){}
};

#endif // TRANSLATOR_H_INCLUDED
