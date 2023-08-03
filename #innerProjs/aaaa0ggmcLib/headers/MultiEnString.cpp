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

///astring
bool astring::transcodeAuto = true;

bool astring::SetAutoTranslate(bool value){
    bool pre = transcodeAuto;
    transcodeAuto = value;
    return pre;
}
bool astring::GetAutoTranslate(){
    return transcodeAuto;
}

bool astring::setAutoForward(bool value){
    bool pre = forwardAuto;
    forwardAuto = value;
    return pre;
}
bool astring::getAutoForward(){
    return forwardAuto;
}

size_t astring::length(unsigned target){
    switch(target){
    case AENC_GBK32:
        return gbk32.length();
    case AENC_UTF8:
        return utf8.length();
    case AENC_UTF16:
        return utf16.length();
    default:
        #ifdef __SEH__
        throw string("Astring has handled an unknown encoding type!");
        #else
        return 0;
        #endif // __SEH__
    }
}
//can also call length(AENC_UTF16)
size_t astring::wlength(){
    return length(AENC_UTF16);
}

const char* astring::utf8p(){return utf8.c_str();}
const char* astring::gbk32p(){return gbk32.c_str();}
const wchar_t* astring::utf16p(){return utf16.c_str();}

void astring::markDirty(){
    dirty = true;
}
bool astring::isDirty(){
    return dirty;
}

istream& operator>>(istream& i,astring& a){
    i >> a.utf8;
    a.enc_now = AENC_UTF8;
    a.markDirty();
    if(a.getAutoForward())a.transcode(a.enc_now);
    return i;
}
ostream& operator<<(ostream& o,astring& a){
    a.transcode(a.enc_now);
    o << a.utf8;
    return o;
}
wistream& operator>>(wistream&wi,astring& a){
    wi >> a.utf16;
    a.enc_now = AENC_UTF16;
    a.markDirty();
    if(a.getAutoForward())a.transcode(a.enc_now);
    return wi;
}
wostream& operator<<(wostream&wo,astring& a){
    a.transcode(a.enc_now);
    wo << a.utf16;
    return wo;
}
istream& operator+(istream&i,astring&a){
    i >> a.gbk32;
    a.enc_now = AENC_GBK32;
    a.markDirty();
    if(a.getAutoForward())a.transcode(a.enc_now);
    return i;
}
ostream& operator+(ostream&o,astring& a){
    a.transcode(a.enc_now);
    o << a.gbk32;
    return o;
}

wstring astring::utf82utf16(const string& s){
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(s);
}
string astring::utf82gbk32(const string&s){
    return Util::UTF8ToGBK(s);
}

string astring::gbk322utf8(const string&s){
    return Util::GBKToUTF8(s);
}
string astring::utf162utf8(const wstring&s){
    return wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t >{}.to_bytes(s);
}

wstring astring::gbk322utf16(const string& s){
    return utf82utf16(gbk322utf8(s));
}
string astring::utf162gbk32(const wstring& s){
    return utf82gbk32(utf162utf8(s));
}

const wchar_t* astring::operator=(wchar_t* s){return assign(s);}
wstring& astring::operator=(wstring&s){return assign(s);}
astring& astring::operator=(astring&s){return assign(s);}
string& astring::operator=(string&s){return assign(s,AENC_UTF8);}
const char * astring::operator=(const char *s){return assign(s,AENC_UTF8);}

const wchar_t* astring::operator+=(wchar_t*s){return append(s);}
wstring& astring::operator+=(wstring&s){return append(s);}
astring& astring::operator+=(astring&s){return i_append(s);}
string& astring::operator+=(string&s){return append(s,AENC_UTF8);}
const char * astring::operator+=(const char *s){return append(s,AENC_UTF8);}

const char* astring::assign(const char * str,unsigned int c){
    #ifdef __SEH__
        if(!str)throw string("Cannot pass NULL to astring!");
        else if(c == AENC_UTF16)throw string("Unknown type has passed to astring");
    #else
        if(!str)str = "";
        else if(c == AENC_UTF16)c = AENC_UTF8;
    #endif // __SEH__
    enc_now = c;
    markDirty();
    switch(c){
    case AENC_UTF8:
        utf8 = str;
        break;
    case AENC_GBK32:
        gbk32 = str;
        break;
    default:
        #ifdef __SEH__
        throw string("Astring has handled a wrong encoding type!");
        #endif // __SEH__
        break;
    }
    if(forwardAuto)transcode(c);
    return str;
}
const wchar_t* astring::assign(const wchar_t * str){
    #ifdef __SEH__
    if(!str)throw string("Cannot pass NULL to astring!");
    #else
        if(!str)str = L"";
    #endif // __SEH__
    enc_now = AENC_UTF16;
    markDirty();
    utf16 = str;
    if(forwardAuto)transcode(AENC_UTF16);
    return str;
}
string& astring::assign(string&s,unsigned int c){assign(s.c_str(),c);return s;}
wstring& astring::assign(wstring& w){assign(w.c_str());return w;}
astring& astring::assign(astring&a){
    enc_now = a.enc_now;
    utf8 = a.utf8;
    utf16 = a.utf16;
    gbk32 = a.gbk32;
    dirty = a.dirty;
    if(dirty && forwardAuto)transcode(enc_now);
    return a;
}

const char* astring::append(const char * str,unsigned int c){
    #ifdef __SEH__
        if(!str)throw string("Cannot pass NULL to astring!");
        else if(c == AENC_UTF16)throw string("Unknown type has passed to astring");
    #else
        if(!str)str = "";
        else if(c == AENC_UTF16)c = AENC_UTF8;
    #endif // __SEH__
    enc_now = c;
    markDirty();
    switch(c){
    case AENC_UTF8:
        utf8 += str;
        break;
    case AENC_GBK32:
        gbk32 += str;
        break;
    default:
        #ifdef __SEH__
        throw string("Astring has handled a wrong encoding type!");
        #endif // __SEH__
        break;
    }
    if(forwardAuto)transcode(c);
    return str;
}
const wchar_t* astring::append(const wchar_t * str){
    #ifdef __SEH__
    if(!str)throw string("Cannot pass NULL to astring!");
    #else
        if(!str)str = L"";
    #endif // __SEH__
    enc_now = AENC_UTF16;
    markDirty();
    utf16 += str;
    if(forwardAuto)transcode(AENC_UTF16);
    return str;
}
string& astring::append(string&s,unsigned int c){append(s.c_str(),c);return s;}
wstring& astring::append(wstring& w){append(w.c_str());return w;}
astring& astring::i_append(astring&a){
    astring copya = a;
    copya.transcode(a.enc_now);

    transcode(enc_now);
    utf8 += copya.utf8;
    markDirty();
    transcode(AENC_UTF8);
    return a;
}
astring& astring::append(astring& a){return i_append(a);}

astring::astring(const char * str,unsigned int c){
    forwardAuto = transcodeAuto;
    assign(str,c);
}
astring::astring(const wchar_t * str){
    forwardAuto = transcodeAuto;
    assign(str);
}
astring::astring(string& s,unsigned int c){astring(s.c_str(),c);}
astring::astring(wstring& w){astring(w.c_str());}
astring::astring(astring& a){
    forwardAuto = transcodeAuto;
    assign(a);
}

void astring::transcode(unsigned int c){
    if(!dirty)return;
    if(c == AENC_DEF)c = enc_now;
    //check code
    if(c == AENC_GBK32)utf8 = gbk322utf8(gbk32);
    else if(c == AENC_UTF16)utf8 = utf162utf8(utf16);

    //transcode
    if(c != AENC_GBK32){
        gbk32 = utf82gbk32(utf8);
    }
    if(c != AENC_UTF16){
        utf16 = utf82utf16(utf8);
    }
    dirty = false;
}
