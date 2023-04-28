#ifndef SPDLOG_H_INCLUDED
#define SPDLOG_H_INCLUDED
#include <memory>
#include <string>

#ifdef SIMPLE_SPD
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#endif // SIMPLE_SPD

using namespace std;

#define LOG_TRACE 0
#define LOG_DEBUG 1
#define LOG_INFO 2
#define LOG_WARN 3
#define LOG_ERROR 4
#define LOG_CRITICAL 5
#define LOG_OFF 6

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
    class DLL_EXPORT LogSaver{
    private:
        bool output2c;
        bool openedStoring;
        bool m_inited;
        #ifdef SIMPLE_SPD
        std::shared_ptr<spdlog::logger> lgr;
        spdlog::level::level_enum lv;
        #else
        std::shared_ptr<void> lgr;
        int lv;
        #endif // SIMPLE_SPD
    public:
        LogSaver(bool otc = true);
        ~LogSaver();
        void SetFileOutput(std::string path,std::string head = "");
        void flush();
        void close();
        void setLevel(int lv);
        void operator <<(std::string v);
        void operator <<(int v);
        void operator <<(double v);
        void operator <<(float v);
        void operator <<(const char * v);
        void OutputToConsole(bool value = true);
        bool getStatus();
        void closeStoring();
    };
}

#ifdef __cplusplus
}
#endif

#endif // SPDLOG_H_INCLUDED
