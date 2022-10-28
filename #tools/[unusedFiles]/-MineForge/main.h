#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef DLL_USING
#define MFG_STATUS_SUC 0
#define MFG_STATUS_FAIL 1
typedef int (*int_void)(void);
#else

#include <windows.h>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

DLL_EXPORT int CheckPython(void);

#ifdef __cplusplus
}
#endif

#endif // DLL_USING

#endif // __MAIN_H__
