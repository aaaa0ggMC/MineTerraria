#define SIMPLE_SPD
#include "expanded.h"

using namespace std;
using namespace spdlog;

#ifdef BUILD_ON_WINDOWS
string _Windows_getCPUInfo(){
    long lRet;
	HKEY hKey;
	CHAR tchData[1024];
	DWORD dwSize;

	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("Hardware\\Description\\System\\CentralProcessor\\0"),0,KEY_QUERY_VALUE,&hKey);

	if(lRet == ERROR_SUCCESS){
		dwSize = 1024;
		lRet = RegQueryValueEx(hKey,_T("ProcessorNameString"),0,0,(LPBYTE)tchData,&dwSize);

		tchData[dwSize] = '\0';
		RegCloseKey(hKey);
		if(lRet == ERROR_SUCCESS){
			return string(tchData);
		}else{
			return "Unknown";
		}
	}
	return "Unknown";
}
#endif // BUILD_ON_WINDOWS

string translateSeconds(int msecs){
    int sec = msecs%60;
    msecs /= 60;
    int min = msecs % 60;
    msecs /= 60;
    int hour = msecs % 60;
    msecs /= 60;
    int day = msecs % 24;
    msecs /= 24;
    int year = msecs % 356;
    string ret = "";
    if(year != 0)ret += to_string(year) + "y ";
    if(day != 0)ret += to_string(day) + "d ";
    if(hour != 0)ret += to_string(hour) + "h ";
    if(min != 0)ret += to_string(min) + "m ";
    if(sec != 0)ret += to_string(sec) + "s";
    return ret;
}

MemTp GetCurrentMemoryUsage(){
    uint64_t mem = 0, vmem = 0;
    PROCESS_MEMORY_COUNTERS pmc;

    // get process hanlde by pid
    HANDLE process = GetCurrentProcess();
    if (GetProcessMemoryInfo(process, &pmc, sizeof(pmc)))
    {
        mem = pmc.WorkingSetSize;
        vmem = pmc.PagefileUsage;
    }
    CloseHandle(process);

    // use GetCurrentProcess() can get current process and no need to close handle

    // convert mem from B to MB
    return {(float)(mem / 1024.0 / 1024.0),(float)(vmem/1024.0/1024.0)};
}

GlMem GetGlobalMemoryUsage(){
    MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	DWORDLONG physical_memory = statex.ullTotalPhys / (1024 * 1024);
	DWORDLONG virtual_memory = (DWORD)statex.ullAvailVirtual / (1024 * 1024);
	DWORDLONG usePhys = physical_memory - (statex.ullAvailPhys / (1024 * 1024));

	float percent_memory = (float)usePhys / (float)physical_memory;
	return {percent_memory,(float)physical_memory,(float)virtual_memory,(float)usePhys};
}

void LogSaver::initStoring(string storeIn){
    lgr = spdlog::basic_logger_mt("UnlimitedLife",storeIn,true);
    this->setLevel(LOG_INFO);
    m_inited = true;
}
void LogSaver::flush(){
    if(!m_inited)return;
    lgr->flush();
}
void LogSaver::close(){
    if(!m_inited)return;
    this->flush();
    m_inited = false;
}
LogSaver::~LogSaver(){
    ///Close
    this->close();
}

void LogSaver::setLevel(unsigned int lv){
    switch(lv){
    case LOG_INFO:
        this->lv = level::info;
        break;
    case LOG_ERRO:
        this->lv = level::err;
        break;
    case LOG_CRIT:
        this->lv = level::critical;
        break;
    case LOG_WARN:
        this->lv = level::warn;
        break;
    }
}

void LogSaver::operator <<(string v){
    if(openedStoring){
        #ifdef LOG_AS_CON
        spdlog::log(lv,v);
        #endif // LOG_AS_CON
        if(m_inited)lgr->log(lv,v);
    }
}
void LogSaver::operator <<(int v){(*this) << to_string(v);}
void LogSaver::operator <<(double v){(*this) << to_string(v);}
void LogSaver::operator <<(float v){(*this) << to_string(v);}
void LogSaver::operator <<(char * v){(*this) << string(v);}

CPUInfo::CPUInfo(){
    #ifdef BUILD_ON_WINDOWS
    this->CpuID = _Windows_getCPUInfo();
    #endif // BUILD_ON_WINDOWS
}
