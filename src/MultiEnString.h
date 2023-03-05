#ifndef MULTIENSTRING_H_INCLUDED
#define MULTIENSTRING_H_INCLUDED
#include "kernel.h"

using namespace std;

class MultiEnString{
public:
    enum EncType{
        GBK,Utf8,Utf16,Unicode
    };
    MultiEnString(string str,EncType);
    MultiEnString(wstring str,EncType);
    string GetUtf8();
    string GetGBK();
    wstring GetUTF16();
    wstring GetUnicode();

    string utf8InnerData;
};

#endif // MULTIENSTRING_H_INCLUDED
