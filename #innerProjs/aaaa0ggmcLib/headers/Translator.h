#ifndef TRANSLATOR_H_INCLUDED
#define TRANSLATOR_H_INCLUDED

#include <string>
#include <vector>
#include <unordered_map>
#include <MultiEnString.h>

#define VERIFY_TOKEN "Language"
#define ACCESS_TOKEN "Access"

#define TEXT_MAX_SIZE 65536

#define MultiTranslate(obj,oid,odv,enc,...) \
    [&](Translator o,string id,string dv){\
        string reps = dv.compare("")?dv:id;\
        if(!o.currentTranslates)return MultiEnString(reps);\
        bool rep = false;\
        auto iter = o.currentTranslates->find(id);\
        if(iter == o.currentTranslates->end())rep = true;\
        string r= "";\
        char * buf = new char[TEXT_MAX_SIZE];memset(buf,0,sizeof(char)*TEXT_MAX_SIZE);\
        sprintf_s(buf,sizeof(char) * (TEXT_MAX_SIZE-1),rep?reps.c_str():((iter->second).c_str()),__VA_ARGS__);\
        buf[TEXT_MAX_SIZE-1] = '\0';\
        r = buf;\
        delete [] buf;\
        return MultiEnString(r);\
    }(obj,oid,odv)
/*
    [&](Translator o,string id,string dv){
        string reps = dv.compare("")?dv:id;
        if(!o.currentTranslates)return MultiEnString(reps);
        bool rep = false;
        auto iter = o.currentTranslates->find(id);
        if(iter == o.currentTranslates->end())rep = true;
        string r= "";
        char * buf = new char[TEXT_MAX_SIZE];memset(buf,0,sizeof(char)*TEXT_MAX_SIZE);
        sprintf_s(buf,sizeof(char) * (TEXT_MAX_SIZE-1),rep?reps:((*iter).c_str()),__VA_ARGS__);
        buf[TEXT_MAX_SIZE-1] = '\0';
        r = buf;
        delete [] buf;
        return MultiEnString(r);
    }(obj,oid,odv)
*/

#ifndef DLL_EXPORT
#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif
#endif // DLL_EXPORT

#ifdef __cplusplus
extern "C"
{
#endif

namespace alib{

    ///文字多语言支持
    class DLL_EXPORT Translator{
    public:
        using TransMap = unordered_map<::string,::string>;
        ///返回得到的语言数量
        TransMap *currentTranslates;
        unordered_map<::string,TransMap> summTrans;
        ::string defaultKey;
        TransMap * defaultTranslates;

        Translator():defaultKey("en_us"){
            currentTranslates = NULL;
            defaultTranslates = NULL;
        }

        MultiEnString Translate(::string id,::string def = "",MultiEnString::EncType enc = MultiEnString::UTF8);
        void SetDefaultKey(const ::string & s);
        void SetDefaultKey(const char * s);
        int LoadTranslate(::string lan_id);
        int LoadTranslateFiles(::string path);
    };

}

#ifdef __cplusplus
}
#endif

#endif // TRANSLATOR_H_INCLUDED
