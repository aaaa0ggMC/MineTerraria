#ifndef DLLKERNEL_H_INCLUDED
#define DLLKERNEL_H_INCLUDED
#include <windows.h>
#include <string>
#include <map>
#include <vector>
#include <time.h>

using namespace std;

/*  To use this exported function of dll, include this header
 *  in your project.
 */


#ifdef BUILD_DLL
    #define MOD_API __declspec(dllexport)
#else
    #define MOD_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

#define UNSETTED_UUID ""
#define MOD_VER 0x00000001

///Event Support
struct EventArgs{
    enum Events{
        Empty
    };
    int uuid;
    map<string,void*> args;
    EventArgs(){
        uuid = Empty;
    }
};
struct EventRet{
    vector<string> rets;
    bool boolValue;
    EventRet(){
        rets = {};
        boolValue = false;
    }
};
typedef EventRet (*EventFunc)(EventArgs);
struct MEvent{
    map<string,EventFunc> funcs;
};
///When a event function returns void
#define ERET_VOID EventRet()
///
typedef void(*VOID_FUNC)(void);

struct Version{
    string BaseVersion;
    short major;
    short minor;
    string add;
    Version(string base = "",short maj = 0,short mino = 1,string a = ""){
        this->BaseVersion = base;
        this->major = maj;
        this->minor = mino;
        add = a;
    }
    string toString(){
        return BaseVersion + to_string(major) + "." + to_string(minor) + add;
    }
};

struct Logger{
    string log;
    Logger(){
        log = "";
    }
};

inline void pushLog(Logger * l,string s){
    l->log +=  s;
}

#define plg(l,s) pushLog(&l,s)

struct TestValues{
    VOID_FUNC fn {NULL};
    int * iv {NULL};
    void * vv {NULL};
};

struct ModInfo : public Logger{
    string name;
    string author;
    string Description;
    string packageUUID;
    Version version;
    bool thisObjIsValid;
    int dllKernelVersion;
    TestValues test;
    bool failed;
    ModInfo(string uuid = UNSETTED_UUID,string n = "NONAME",string au = "NOAU_TH",string des = "????",Version v = Version()){
        name = n;
        author = au;
        Description = des;
        packageUUID = uuid;
        version = v;
        thisObjIsValid = false;
        log = "";
        dllKernelVersion = MOD_VER;
        test = TestValues();
    }
    void DoVaild(){
        thisObjIsValid = true;
    }
};

struct BasicInfo{
    int Language;
    int dllKernelVersion;
    const static int Chinese = 0,English = 1;
};

struct InitInfo{
    /**
    *Supported Hooks
    *     Description           HookName       ExpectReturn                       Return Desc
    *Main Menu Back Draw Hook:   draw.menubk   boolean value     true:draw term finished;false:draw term continue
                            Also supports string list:Such as discard system draw,add a ret string "Discard.SystemUI"
    */
    MEvent hooks;
    bool hasLog;
    string log;
};

struct InitInfoOut{
    vector<string> * titleString;
    InitInfoOut(vector<string>* ss = NULL){
        titleString = ss;
    }
};

#define GET_MOD_DATA_FUNC "GetModInfo"
#define INIT_MOD_FUNC "InitializeMod"
ModInfo MOD_API GetModInfo(BasicInfo);
InitInfo MOD_API InitializeMod(InitInfoOut);
typedef ModInfo (*_GetModInfo)(BasicInfo);
typedef InitInfo (*_InitMod)(InitInfoOut);

#ifdef __cplusplus
}
#endif
#endif // DLLKERNEL_H_INCLUDED


#ifdef DLL_TOOL
namespace dllTool{
    namespace encoding{
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
    }
}
#endif // DLL_TOOL

#ifndef DLL_KERNEL_ONLY_FOR_DLL
#define DLL_KERNEL_ONLY_FOR_DLL
///Application Do not Needed But DLL Needed
#define DLL_BASIC extern "C" MOD_API BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)\
{\
    switch (fdwReason)\
    {\
        case DLL_PROCESS_ATTACH:\
            break;\
        case DLL_PROCESS_DETACH:\
            break;\
        case DLL_THREAD_ATTACH:\
            break;\
        case DLL_THREAD_DETACH:\
            break;\
    }\
    return TRUE;\
}
#endif // DLL_KERNEL_ONLY_FOR_DLL
