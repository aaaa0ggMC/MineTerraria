#include "MultiEnString.h"
#include <aaa_util.h>
#include <locale>
#include <codecvt>
#include <string.h>
#include <windows.h>

using namespace std;
using namespace alib;

string ANSIToUTF8(string in){
    //定义输入值并获取其长度
    string buildRet = "";
    char * input_string=(char *)in.c_str();
    int in_size= strlen(input_string);


    /*映射一个字符串到一个宽字符（unicode）的字符串。由该函数映射的字符串没必要是多字节字符组。
       CP_ACP：ANSI代码页（简体中文Windows操作系统中，ANSI 编码代表 GBK 编码）*/
    //先获取宽字符串长度并创建，再以实际值执行函数
    int wide_size=MultiByteToWideChar(CP_ACP, 0, input_string, in_size, NULL, 0);
    wchar_t * wide_string = (wchar_t * ) malloc(wide_size*sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, input_string, in_size, wide_string, wide_size);


    /*把宽字符串转换成指定的新的字符串，如ANSI，UTF8等，新字符串不必是多字节字符集。
       CP_UTF8：使用UTF-8转换*/
    int utf8_size = WideCharToMultiByte(CP_UTF8, 0, wide_string, wide_size, NULL, 0, NULL, NULL);
    char *utf8_string = (char * ) malloc(utf8_size);
    WideCharToMultiByte(CP_UTF8, 0, wide_string, wide_size, utf8_string, utf8_size, NULL, NULL);
    free(wide_string);

    buildRet = string(utf8_string);
    free(utf8_string);
    return buildRet;
}

MultiEnString::MultiEnString(string str,EncType e){
    if(e == GBK){
        utf8InnerData = Util::GBKToUTF8(str);
    }else if(e == UTF8)utf8InnerData = str;
}

MultiEnString::MultiEnString(wstring str,EncType e){
    if(e == Unicode){
        ///TODO：还要完善 Required to do it well
        utf8InnerData = std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t >{}.to_bytes(str);
    }else if(e == UTF16){
        utf8InnerData = std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t >{}.to_bytes(str);
    }
}

string MultiEnString::GetUTF8(){
    return utf8InnerData;
}

string MultiEnString::GetGBK(){
    return Util::UTF8ToGBK(utf8InnerData);
}

wstring MultiEnString::GetUTF16(){
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide_string = converter.from_bytes(utf8InnerData);
    return wide_string;
}

wstring MultiEnString::GetUnicode(){
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wide_string = converter.from_bytes(utf8InnerData);
    return wide_string;
}
