#ifndef CCLOCK_H_INCLUDED
#define CCLOCK_H_INCLUDED
#include <windows.h>

namespace cck{
    struct TMST0{
        DWORD all;
        DWORD offset;
    };

    class Clock{
    public:
        Clock(bool start = true);
        void Start();
        TMST0 Stop();
        DWORD GetALLTime();//Do not set pre time
        DWORD GetOffset();//Set Pre Time
        TMST0 Now();//Do not reset preTime
        TMST0 Pause();
        void Resume();
    private:
        DWORD m_pauseGained;
        DWORD m_StartTime;
        DWORD m_PreTime;
        bool m_start;
        bool m_paused;
    };
}


#endif // CCLOCK_H_INCLUDED
