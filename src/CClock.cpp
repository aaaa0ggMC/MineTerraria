#include "CClock.h"

using namespace cck;

#define AssertSt _d(m_useHTimer?timeGetTimeEx(m_cpuFeq):timeGetTime())

bool cck::Clock::static_lock = false;
LARGE_INTEGER cck::Clock::m_cpuFeq = {0};
bool cck::Clock::m_useHTimer = false;
bool cck::Clock::useHTimer = false;

Clock::Clock(bool start){
    this->m_StartTime = this->m_PreTime = 0;
    this->m_pauseGained = 0;
    this->m_start = false;
    this->m_paused = false;
    if(!static_lock){
        static_lock = true;
        if(QueryPerformanceFrequency(&m_cpuFeq)){
            m_useHTimer = useHTimer = true;
        }else {
            m_useHTimer = useHTimer = false;
        }
    }
    if(start){
        this->Start();
    }
}

void Clock::Start(){
    if(m_start)return;
    this->m_start = true;
    this->m_StartTime = AssertSt;
}

TMST0 Clock::Pause(){
    if(m_paused)return {0,0};
    m_paused = true;
    TMST0 ret = Now();
    m_pauseGained = ret.all;
    Stop();
    return ret;
}

void Clock::Resume(){
    if(!m_paused)return;
    Start();
    this->m_PreTime = AssertSt;
    m_paused = false;
}

TMST0 Clock::Now(){
    if(!m_start)return {0,0};
    TMST0 t;
    t.all = AssertSt - this->m_StartTime + m_pauseGained;
    t.offset = AssertSt - this->m_PreTime;
    return t;
}

double Clock::GetALLTime(){
    if(!m_start)return 0;
    return Now().all;
}

double Clock::GetOffset(){
    if(!m_start)return 0;
    double off = Now().offset;
    this->m_PreTime = AssertSt;
    return off;
}

TMST0 Clock::Stop(){
    if(!m_start)return {0,0};
    TMST0 rt = Now();
    this->m_StartTime = 0;
    this->m_start = false;
    if(!m_paused){
        m_pauseGained = 0;
    }
    return rt;
}
