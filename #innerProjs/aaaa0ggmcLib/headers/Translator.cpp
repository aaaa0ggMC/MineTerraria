#include "Translator.h"
#include <aaa_util.h>
#include <rapidjson.h>
#include <unordered_map>
#include <algorithm>
#include <windows.h>

using namespace std;
using namespace alib;

Translator* Translator::instance;

int AnalyseAFile(std::string path,unordered_map<std::string,std::string> & d){
    alib::Document doc;
    int sz = Util::file_size((char*)path.c_str());
    FILE * f = fopen(path.c_str(),"r");
    if(f){
        char * data = new char[sz];
        memset(data,0,sizeof(char) * sz);
        fread(data,sizeof(char),sz,f);
        std::string str = data;
        delete [] data;
        if(!doc.Parse(str.c_str()))return -2;
        doc.ForEach([](string n,string v,void * r){
            unordered_map<string,string> & d = *(unordered_map<string,string>*)r;
            d.insert(make_pair(n,Util::GetTranslateString(v)));
        },&d);
        return 0;
    }
    return -1;
}

int Translator::LoadTranslateFiles(::string path){
    std::vector<::string> fs;
    Util::getFileNames(path,fs);
    for(::string ss : fs){
        ::string tail = ss.substr(ss.find_last_of('.')+1);
        if(tail.compare("json")){
            continue;
        }
        unordered_map<::string,::string> trs;
        ///大错误！！！AnalyseAFile前面没加上非符号浪费了我好多时间
        if(!AnalyseAFile(ss,trs)){
            if(trs.find(VERIFY_TOKEN) != trs.end()){
                if(trs.find(ACCESS_TOKEN) != trs.end()){
                    ///防止占用InnerUS
                    if(trs[ACCESS_TOKEN].compare(""))summTrans.insert(make_pair(trs[ACCESS_TOKEN],trs));
                }
            }
        }
    }
    return 0;
}

int Translator::LoadTranslate(string id){
    currentTranslates = NULL;
    ///使用系统翻译
    if(!id.compare(""))return 0;
    auto iter = summTrans.find(id);
    if(iter == summTrans.end()){
        iter = summTrans.find(defaultKey);
        if(iter == summTrans.end()){
            return -1;
        }
        else{
            currentTranslates = &(iter->second);
            if(buildLocalDataB){
                BuildLocalData(localData,currentTranslates);
            }
            return -2;
        }
    }
    currentTranslates = &(iter->second);
    if(buildLocalDataB){
        BuildLocalData(localData,currentTranslates);
    }
    return 0;
}

MultiEnString Translator::Translate(string id,string def,MultiEnString::EncType enc){
    string reps = def.compare("")?def:id;
    if(!currentTranslates)return MultiEnString(reps,enc);
    auto iter = currentTranslates->find(id);
    if(iter == currentTranslates->end())return MultiEnString(reps,enc);
    return MultiEnString(iter->second,MultiEnString::UTF8);
}

MultiEnString Translator::iTranslate(string id,string def,MultiEnString::EncType enc){
    if(!instance)return MultiEnString("",MultiEnString::UTF8);
    return instance->Translate(id,def,enc);
}

void Translator::SetDefaultKey(const char * s){if(s)defaultKey = s;}
void Translator::SetDefaultKey(const string & s){defaultKey = s;}

void Translator::set(Translator * t){
    instance = t;
}

Translator::Translator(bool v,bool x){
    if(v && (instance == NULL)){
        instance = this;
    }
    empty_translate.assign("[null content]",AENC_UTF8);
    buildLocalDataB = !x;
    defaultKey ="en_us";
    currentTranslates = NULL;
    //defaultTranslates = NULL;
    strBuffer.resize(TEXT_MAX_SIZE);
}

Translator* Translator::get(){return instance;}

MultiEnString Translator::Translate(MultiEnString me,va_list va){
    string s = me.GetUTF8();
    int sz = vsnprintf((char*)strBuffer.c_str(),TEXT_MAX_SIZE,s.c_str(),va);
    return MultiEnString(strBuffer.substr(0,sz),MultiEnString::UTF8);
}

MultiEnString Translator::MTranslate(string& id,string& def,MultiEnString::EncType e,va_list ap){
    string reps = def.compare("")?def:id;
    if(!currentTranslates)return Translate(MultiEnString(reps,e),ap);
    auto iter = currentTranslates->find(id);
    if(iter == currentTranslates->end())return Translate(MultiEnString(reps,e),ap);
    return Translate(MultiEnString(iter->second,e),ap);
}

MultiEnString Translator::MTranslate(string id,string def,MultiEnString::EncType e,...){
    va_list vl;
    va_start(vl,e);
    MultiEnString md = MTranslate(id,def,e,vl);
    va_end(vl);
    return md;
}


MultiEnString Translator::iMTranslate(string id,string def,MultiEnString::EncType e,...){
    if(!instance)return MultiEnString("",MultiEnString::UTF8);
    va_list vl;
    va_start(vl,e);
    MultiEnString md = instance->MTranslate(id,def,e,vl);
    va_end(vl);
    return md;
}

void Translator::BuildLocalData(Translator::LocalTransMap & l,Translator::TransMap * c){
    for(auto&[key,pt] : l){
        delete pt;
    }
    l.clear();
    if(!c)return;
    for(auto&[key,value] : *c){
        l.insert(make_pair(key,new astring(value,AENC_UTF8)));
    }
}


astring& Translator::FTranslate(const string & id){
    auto iter = localData.find(id);
    if(iter == localData.end())return empty_translate;
    return *(iter->second);
}
astring& Translator::FTranslate(const char *id){
    auto iter = localData.find(id);
    if(iter == localData.end())return empty_translate;
    return *(iter->second);
}
