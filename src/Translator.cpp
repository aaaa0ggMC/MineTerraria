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
        string head = ss.substr(ss.find_last_of('\\')+1,ss.find_last_of('.')-ss.find_last_of('\\')-1);
        map<string,string> trs;
        if(AnalyseAFile(ss,trs)){
            summTrans.insert(make_pair(head,trs));
        }
    }
    return 0;
}

int Translator::LoadTranslate(string id,string defId){
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
