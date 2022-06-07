#include "CClock.h"

using namespace cck;

Clock::Clock(bool start){
    this->m_StartTime = this->m_PreTime = 0;
    this->m_pauseGained = 0;
    this->m_start = false;
    this->m_paused = false;
    if(start){
        this->Start();
    }
}

void Clock::Start(){
    if(m_start)return;
    this->m_start = true;
    this->m_StartTime = timeGetTime();
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
    m_paused = false;
}

TMST0 Clock::Now(){
    if(!m_start)return {0,0};
    TMST0 t;
    t.all = timeGetTime() - this->m_StartTime + m_pauseGained;
    t.offset = timeGetTime() - this->m_PreTime;
    return t;
}

DWORD Clock::GetALLTime(){
    if(!m_start)return 0;
    return Now().all;
}

DWORD Clock::GetOffset(){
    if(!m_start)return 0;
    DWORD off = Now().offset;
    this->m_PreTime = timeGetTime();
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
