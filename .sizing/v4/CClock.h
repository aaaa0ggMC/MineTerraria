#ifndef CCLOCK_H_INCLUDED
#define CCLOCK_H_INCLUDED
#include <windows.h>
#define _d(x) ((double)(x))

namespace cck{
    struct TMST0{
        double all;
        double offset;
    };

    class Clock{
    public:
        static bool m_useHTimer;
        static LARGE_INTEGER m_cpuFeq;
        static bool static_lock;
        static bool useHTimer;
        Clock(bool start = true);
        void Start();
        TMST0 Stop();
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
}

inline double timeGetTimeEx(LARGE_INTEGER & feq){
    LARGE_INTEGER lg;
    QueryPerformanceCounter(&lg);
    return ((double)lg.QuadPart/(double)feq.QuadPart * 1000);
}


#endif // CCLOCK_H_INCLUDED
