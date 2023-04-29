#include "MultiEnString.h"
#include <aaa_util.h>
#include <locale>
#include <codecvt>
#include <string.h>
#include <windows.h>

using namespace std;
using namespace alib;

string ANSIToUTF8(string in){
    //��������ֵ����ȡ�䳤��
    string buildRet = "";
    char * input_string=(char *)in.c_str();
    int in_size= strlen(input_string);


    /*ӳ��һ���ַ�����һ�����ַ���unicode�����ַ������ɸú���ӳ����ַ���û��Ҫ�Ƕ��ֽ��ַ��顣
       CP_ACP��ANSI����ҳ����������Windows����ϵͳ�У�ANSI ������� GBK ���룩*/
    //�Ȼ�ȡ���ַ������Ȳ�����������ʵ��ִֵ�к���
    int wide_size=MultiByteToWideChar(CP_ACP, 0, input_string, in_size, NULL, 0);
    wchar_t * wide_string = (wchar_t * ) malloc(wide_size*sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, input_string, in_size, wide_string, wide_size);


    /*�ѿ��ַ���ת����ָ�����µ��ַ�������ANSI��UTF8�ȣ����ַ��������Ƕ��ֽ��ַ�����
       CP_UTF8��ʹ��UTF-8ת��*/
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
        ///TODO����Ҫ���� Required to do it well
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
