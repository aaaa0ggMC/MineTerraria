#ifndef MULTIENSTRING_H_INCLUDED
#define MULTIENSTRING_H_INCLUDED
#include <string>

using namespace std;

class MultiEnString{
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

#endif // MULTIENSTRING_H_INCLUDED
