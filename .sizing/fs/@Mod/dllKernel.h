#ifndef DLLKERNEL_H_INCLUDED
#define DLLKERNEL_H_INCLUDED
#include <windows.h>
#include <string>

using namespace std;

/*  To use this exported function of dll, include this header
 *  in your project.
 */


#ifdef BUILD_DLL
    #define MT_API __declspec(dllexport)
#else
    #define MT_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

struct ModInfo{
    string name;
    string author;
    string Description;
};

struct BasicInfo{
    int Language;
    const static int Chinese = 0,English = 1;
};

#define GET_MOD_DATA_FUNC "GetModInfo"
ModInfo MT_API GetModInfo(BasicInfo);
typedef ModInfo (*_GetModInfo)(BasicInfo);

#ifdef __cplusplus
}
#endif

#endif // DLLKERNEL_H_INCLUDED




#ifndef DLL_KERNEL_ONLY_FOR_DLL
#define DLL_KERNEL_ONLY_FOR_DLL
///Application Do not Needed But DLL Needed
#define DLL_BASIC extern "C" MT_API BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)\
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
