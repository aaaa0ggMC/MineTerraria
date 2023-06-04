#ifndef TRANSLATOR_H_INCLUDED
#define TRANSLATOR_H_INCLUDED

#include <string>
#include <vector>
#include <unordered_map>
#include <MultiEnString.h>
#include <stdarg.h>

#define VERIFY_TOKEN "Language"
#define ACCESS_TOKEN "Access"

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

#define TEXT_MAX_SIZE 65535

namespace alib{

    ///文字多语言支持
    class DLL_EXPORT Translator{
    private:
        static Translator * instance;
        string strBuffer;
    public:
        using TransMap = unordered_map<::string,::string>;
        ///返回得到的语言数量
        TransMap *currentTranslates;
        unordered_map<::string,TransMap> summTrans;
        ::string defaultKey;
        TransMap * defaultTranslates;

        Translator(bool setInstanceIfNULL = false);

        MultiEnString Translate(::string id,::string def = "",MultiEnString::EncType enc = MultiEnString::UTF8);
        static MultiEnString iTranslate(string id,string def = "",MultiEnString::EncType enc = MultiEnString::UTF8);
        MultiEnString MTranslate(::string id,::string def = "",MultiEnString::EncType = MultiEnString::UTF8,...);
        MultiEnString MTranslate(string& id,string& def,MultiEnString::EncType,va_list);
        static MultiEnString iMTranslate(string id,string def,MultiEnString::EncType e,...);
        MultiEnString Translate(MultiEnString me,va_list vl);
        void SetDefaultKey(const ::string & s);
        void SetDefaultKey(const char * s);
        int LoadTranslate(::string lan_id);
        int LoadTranslateFiles(::string path);

        static void set(Translator *);
        static Translator * get();
    };

}

#ifdef __cplusplus
}
#endif

#endif // TRANSLATOR_H_INCLUDED
