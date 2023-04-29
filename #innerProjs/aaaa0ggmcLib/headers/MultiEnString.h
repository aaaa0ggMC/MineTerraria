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

}
#ifdef __cplusplus
}
#endif

#endif // MULTIENSTRING_H_INCLUDED
