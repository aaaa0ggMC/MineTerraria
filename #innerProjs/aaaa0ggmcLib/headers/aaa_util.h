#ifndef AAA_UTIL_H_INCLUDED
#define AAA_UTIL_H_INCLUDED
#include <string>
#include <vector>
#include <fstream>

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
    struct DLL_EXPORT MemTp{float mem;float vmem;};
    struct DLL_EXPORT GlMem{float percent;float phy;float vir;float usephy;};

    struct DLL_EXPORT CPUInfo{
        std::string CpuID;
        CPUInfo();
    };

    class DLL_EXPORT Util{
    public:
        static int colorfulPrint(const string message,int color);
        static string getTime();
        static string Windows_getCPUInfo();
        static void getFileNames(string path, vector<string>& files);
        static std::string GetTranslateString(std::string in);
        static void split(std::vector<std::string> & vct,const std::string & line,const char sep);
        static std::string Trim(std::string & str);
        static void Stringsplit(std::string str,std::string splits,std::vector<std::string>& res);
        static std::string GBKToUTF8(const std::string &strGBK);
        static std::string UTF8ToGBK(const std::string &strUTF8);
        static int file_size(char* filename);
        static bool check_exists(char* filename);
        static std::string readAll(std::ifstream & reader);
        static int writeAll(std::string fth,std::string s);
        static MemTp GetCurrentMemoryUsage();
        static GlMem GetGlobalMemoryUsage();
        static std::string translateSeconds(int msecs);

        operator =(Util&) = delete;
        Util() = delete;
    };
}

#ifdef __cplusplus
}
#endif

#endif // AAA_UTIL_H_INCLUDED
