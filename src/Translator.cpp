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
        //cout << str << endl;
        if(doc.HasParseError())return -2;
        for(auto x = doc.MemberBegin();x < doc.MemberEnd();++x){
            string h = GVToString((*x).name);
            string t = GVToString((*x).value);
            //cout << "Anaed " << h << " " << t << endl;
            d.insert(make_pair(h,t));
        }
        //for(auto x : d){
        //   cout << x.first << " " << x.second << endl;
        //}
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
                    summTrans.insert(make_pair(trs[ACCESS_TOKEN],trs));
                }
            }
        }
    }
    return 0;
}

int Translator::LoadTranslate(string id,string defId){
    //for(auto x : summTrans){
    //    cout << "ST(" << x.first  << ")" << endl;
    //}
    //cout << summTrans.size() << endl;
    if(summTrans.find(id) == summTrans.end()){
        if(summTrans.find(defId) == summTrans.end()){
            currentTranslates.clear();
            return -1;
        }
        else{
            currentTranslates = summTrans[defId];
            return -2;
        }
    }
    currentTranslates = summTrans[id];
    return 0;
}

MultiEnString Translator::Translate(string id,string def,MultiEnString::EncType enc){
    if(currentTranslates.find(id) == currentTranslates.end())return MultiEnString(def,enc);
    //string r = "";
    ///不知道为什么 sprintf(NULL,"...",...)会崩溃，于是干脆用上KERNEL的TEXT_MAX_SIZE:65536
    //char * buf = new char[TEXT_MAX_SIZE];memset(buf,0,sizeof(char)*TEXT_MAX_SIZE);
    //sprintf_s(buf,sizeof(char) * (TEXT_MAX_SIZE-1),);
    //buf[TEXT_MAX_SIZE-1] = '\0';
    //r = buf;
    //delete [] buf;
    return MultiEnString(currentTranslates[id],MultiEnString::Utf8);
}
