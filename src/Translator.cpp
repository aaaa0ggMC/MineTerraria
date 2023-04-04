#include "Translator.h"
#include "helping.h"
#include "@rapidjson/document.h"
#include "@rapidjson/stringbuffer.h"
#include "@rapidjson/writer.h"

using namespace std;
using namespace rapidjson;

string GVToString(Value & v){
    if(v.IsString())return string(v.GetString());
    rapidjson::StringBuffer sbBuf;
    rapidjson::Writer<rapidjson::StringBuffer> jWriter(sbBuf);
    v.Accept(jWriter);
    return std::string(sbBuf.GetString());
}

int AnalyseAFile(string path,map<string,string> & d){
    Document doc;
    doc.SetObject();
    int sz = fileIO::file_size((char*)path.c_str());
    FILE * f = fopen(path.c_str(),"r");
    if(f){
        char * data = new char[sz];
        memset(data,0,sizeof(char) * sz);
        fread(data,sizeof(char),sz,f);
        string str = data;
        delete [] data;
        doc.Parse(str.c_str());
        if(doc.HasParseError())return -2;
        for(auto x = doc.MemberBegin();x < doc.MemberEnd();++x){
            string h = GVToString((*x).name);
            string t = GVToString((*x).value);
            d.insert(make_pair(h,strps::GetTranslateString(t)));
        }
        return 0;
    }
    return -1;
}

int Translator::LoadTranslateFiles(string path){
    vector<string> fs;
    getFileNames(path,fs);
    for(string ss : fs){
        string tail = ss.substr(ss.find_last_of('.')+1);
        if(tail.compare("json")){
            continue;
        }
        map<string,string> trs;
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
