#ifndef DLLKERNEL_H_INCLUDED
#define DLLKERNEL_H_INCLUDED
#include <windows.h>
#include <string>
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

#define GET_MOD_DATA_FUNC "GetModInfo"
ModInfo MOD_API GetModInfo(BasicInfo);
typedef ModInfo (*_GetModInfo)(BasicInfo);

#ifdef __cplusplus
}
#endif

#endif // DLLKERNEL_H_INCLUDED




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
