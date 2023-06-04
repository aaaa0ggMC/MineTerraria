#ifndef SPDLOG_H_INCLUDED
#define SPDLOG_H_INCLUDED
#include <memory>
#include <string>
#include <windows.h>
#include <fstream>
#include <CClock.h>

using namespace std;

#define LOG_TRACE 0x00000001
#define LOG_DEBUG 0x00000010
#define LOG_INFO  0x00000100
#define LOG_WARN  0x00001000
#define LOG_ERROR 0x00010000
#define LOG_CRITI 0x00100000
#define LOG_OFF   0x01000000

#define LOG_FULL (LOG_TRACE | LOG_DEBUG | LOG_INFO | LOG_WARN | LOG_ERROR | LOG_CRITI)
#define LOG_RELE (LOG_INFO | LOG_ERROR | LOG_CRITI)


#define LOG_SHOW_TIME 0x00000001
#define LOG_SHOW_TYPE 0x00000010
#define LOG_SHOW_ELAP 0x00000100
#define LOG_SHOW_THID 0x00001000
#define LOG_SHOW_HEAD 0x00010000
#define LOG_SHOW_PROC 0x00100000

#define LOG_SH_BASIC (LOG_SHOW_TIME | LOG_SHOW_TYPE | LOG_SHOW_ELAP | LOG_SHOW_HEAD)
#define LOG_SH_FULL  (LOG_SH_BASIC | LOG_SHOW_THID | LOG_SHOW_PROC)

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
    struct DLL_EXPORT IData{
        string str;
        int color;
    };

    struct DLL_EXPORT CriticalLock{
        CRITICAL_SECTION * cs;
        CriticalLock(CRITICAL_SECTION &);
        ~CriticalLock();
    };

    class DLL_EXPORT LogSaver{
    private:
        friend class LogFactory;
        bool output2c;
        bool m_inited;
        int mode;
        int showlg;
        ofstream ofs;
        string buffer;
        cck::Clock clk;
        CRITICAL_SECTION cs;
        static LogSaver * instance;
    public:
        LogSaver(bool otc = true,bool setInstanceIfNULL = true,int lg = LOG_FULL);
        ~LogSaver();
        bool SetFileOutput(std::string path);
        void configure(int mode);
        void flush();
        void close();
        void OutputToConsole(bool value = true);
        bool getStatus();
        void showLogs(int logs);
        void log(int level,string& msg,string &head);

        int getMode();
        int getMask();

        string makeMsg(int level,string & msg,string &head,bool ends = true);
        static IData genType(int level);
        static void set(LogSaver*);
        static LogSaver* get();
    };

    class DLL_EXPORT LogFactory{
    private:
        bool hasHead;
        string head;
        LogSaver* i;
    public:
        LogFactory(string head = "",bool useStatic = false,LogSaver* lg = NULL);

        void log(int level,string msg);

        void info(string msg);
        void error(string msg);
        void critical(string msg);
        void debug(string msg);
        void trace(string msg);
        void warn(string msg);
    };
}

#ifdef __cplusplus
}
#endif

#endif // SPDLOG_H_INCLUDED
