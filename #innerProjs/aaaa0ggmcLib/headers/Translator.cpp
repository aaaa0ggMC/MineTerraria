#include "Translator.h"
#include <aaa_util.h>
#include <rapidjson.h>
#include <unordered_map>
#include <windows.h>

using namespace std;
using namespace alib;

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
            return -2;
        }
    }
    currentTranslates = &(iter->second);
    return 0;
}

MultiEnString Translator::Translate(string id,string def,MultiEnString::EncType enc){
    string reps = def.compare("")?def:id;
    if(!currentTranslates)return MultiEnString(reps,enc);
    auto iter = currentTranslates->find(id);
    if(iter == currentTranslates->end())return MultiEnString(reps,enc);
    return MultiEnString(iter->second,MultiEnString::UTF8);
}

void Translator::SetDefaultKey(const char * s){if(s)defaultKey = s;}
void Translator::SetDefaultKey(const string & s){defaultKey = s;}
