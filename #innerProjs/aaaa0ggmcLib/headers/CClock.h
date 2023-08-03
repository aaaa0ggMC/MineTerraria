#ifndef CCLOCK_H_INCLUDED
#define CCLOCK_H_INCLUDED
#include <windows.h>
#define _d(x) ((double)(x))

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

namespace cck{
    struct DLL_EXPORT TMST0{
        double all;
        double offset;
    };

    class DLL_EXPORT Clock{
    public:
        static bool m_useHTimer;
        static LARGE_INTEGER m_cpuFeq;
        static bool static_lock;
        static bool useHTimer;
        Clock(bool start = true);
        void Start();
        bool isStop();
        TMST0 Stop();
        bool checkEslapse(double want);
        bool checkEslapseReset(double want);
        double GetALLTime();//Do not set pre time
        double GetOffset();//Set Pre Time
        TMST0 Now();//Do not reset preTime
        TMST0 Pause();
        void Resume();
    private:
        double m_pauseGained;
        double m_StartTime;
        double m_PreTime;
        bool m_start;
        bool m_paused;
    };
};

#ifdef __cplusplus
}
#endif

#endif // CCLOCK_H_INCLUDED
