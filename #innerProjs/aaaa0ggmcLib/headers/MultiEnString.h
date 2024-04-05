#ifndef MULTIENSTRING_H_INCLUDED
#define MULTIENSTRING_H_INCLUDED
#include <string>

using namespace std;
#ifndef DLL_EXPORT
#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif
#endif // DLL_EXPORT

#ifndef ADISABLE_WARNING
#define WARN(X) [[deprecated(X)]]
#else
#define WARN(X)
#endif // ADISABLE_WARNING

#define AENC_UTF8 0
#define AENC_GBK32 1
#define AENC_UTF16 2
#define AENC_DEF 1000

#define AENC_U82GB(U8) alib::MultiEnString(U8).GetUTF8()
#define AENC_U82UN(U8) alib::MultiEnString(U8).GetUnicode()
#define AENC_U82U16(U8) alib::MultiEnString(U8).GetUTF16()
#define AENC_GB2U8(GB) alib::MultiEnString(GB,alib::MultiEnString::GBK).GetUTF8()
#define AENC_GB2U16(GB) alib::MultiEnString(GB,alib::MultiEnString::GBK).GetUTF16()
#define AENC_GB2UN(GB) alib::MultiEnString(GB,alib::MultiEnString::GBK).GetUnicode()
#define AENC_U162UN(U16) alib::MultiEnString(U16,alib::MultiEnString::UTF16).GetUnicode()
#define AENC_U162GB(U16) alib::MultiEnString(U16,alib::MultiEnString::UTF16).GetGBK()
#define AENC_U162U8(U16) alib::MultiEnString(U16,alib::MultiEnString::UTF16).GetUTF8()
#define AENC_UN2U8(UN) alib::MultiEnString(UN,alib::MultiEnString::Unicode).GetUTF8()
#define AENC_UN2U16(UN) alib::MultiEnString(UN,alib::MultiEnString::Unicode).GetUTF16()
#define AENC_UN2GB(UN) alib::MultiEnString(UN,alib::MultiEnString::Unicode).GetGBK()

#ifdef __cplusplus
extern "C"
{
#endif
namespace alib{

    class DLL_EXPORT MultiEnString{
    public:
        enum EncType{
            GBK,UTF8,UTF16,Unicode
        };
        MultiEnString(string str,EncType = UTF8);
        MultiEnString(wstring str,EncType = UTF16);
        string GetUTF8();
        string GetGBK();
        wstring GetUTF16();
        wstring GetUnicode();

        string utf8InnerData;
    };

    class DLL_EXPORT astring{
        bool dirty;
        //transcode whenever the data changed
        bool forwardAuto;
        //transcode when a string is created
        static bool transcodeAuto;


        astring& i_append(astring&);

    public:
        string utf8,gbk32;
        wstring utf16;
        unsigned int enc_now;

        astring(const char * str = "",unsigned int code = AENC_UTF8);
        astring(const wchar_t * wstr);
        astring(string&,unsigned int=AENC_UTF8);
        astring(wstring&);
        astring(astring&);

        const char * assign(const char * str="",unsigned int code = AENC_UTF8);
        string& assign(string&str,unsigned int code = AENC_UTF8);
        const wchar_t* assign(const wchar_t * str);
        wstring& assign(wstring&);
        astring& assign(astring&);

        const char * append(const char * str="",unsigned int code = AENC_UTF8);
        string& append(string&str,unsigned int code = AENC_UTF8);
        const wchar_t* append(const wchar_t * str);
        wstring& append(wstring&);
        WARN("This function would transcode without permission!")
        astring& append(astring&);

        const wchar_t* operator=(wchar_t*);
        wstring& operator=(wstring&);
        astring& operator=(astring&);
        WARN("Default encode is UTF8 here,use assign() to control it fully,or define a macro named \'ADISABLE_WARNING\' to disable this warning.")
        string& operator=(string&);
        WARN("Default encode is UTF8 here,use assign() to control it fully,or define a macro named \'ADISABLE_WARNING\' to disable this warning.")
        const char * operator=(const char *);

        const wchar_t* operator+=(wchar_t*);
        wstring& operator+=(wstring&);
        WARN("This function would transcode without permission!")
        astring& operator+=(astring&);
        WARN("Default encode is UTF8 here,use assign() to control it fully,or define a macro named \'ADISABLE_WARNING\' to disable this warning.")
        string& operator+=(string&);
        WARN("Default encode is UTF8 here,use assign() to control it fully,or define a macro named \'ADISABLE_WARNING\' to disable this warning.")
        const char * operator+=(const char *);

        void transcode(unsigned int=AENC_DEF);
        void markDirty();
        bool isDirty();
        bool setAutoForward(bool);
        bool getAutoForward();

        const char* utf8p();
        const char* gbk32p();
        const wchar_t* utf16p();

        size_t length(unsigned target = AENC_UTF8);
        //can also call length(AENC_UTF16)
        size_t wlength();

        static wstring utf82utf16(const string&);
        static string utf82gbk32(const string&);

        static string gbk322utf8(const string&);
        static string utf162utf8(const wstring&);

        static wstring gbk322utf16(const string&);
        static string utf162gbk32(const wstring&);

        static bool SetAutoTranslate(bool value);
        static bool GetAutoTranslate();
    };
}
#ifdef __cplusplus
}
#endif

namespace alib{
    //IO
    ///Warning:All these function will transcode automatically without restriction!
    istream& DLL_EXPORT operator>>(istream&,astring& a);
    WARN("Passing utf8,this function would transcode without permission!")
    ostream& DLL_EXPORT operator<<(ostream&,astring& a);
    istream& DLL_EXPORT operator+(istream&,astring& a);
    WARN("Passing gbk32,this function would transcode without permission!")
    ostream& DLL_EXPORT operator+(ostream&,astring& a);
    wistream& DLL_EXPORT operator>>(wistream&,astring& a);
    WARN("This function would transcode without permission!")
    wostream& DLL_EXPORT operator<<(wostream&,astring& a);
}

#endif // MULTIENSTRING_H_INCLUDED
