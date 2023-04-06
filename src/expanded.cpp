#include "expanded.h"

using namespace std;

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

bool LogSaver::initStoring(string storeIn){
    outs(storeIn);
    m_writer.open(storeIn,ios::out | ios::trunc);
    m_inited = m_writer.is_open();
    return m_inited;
}
bool LogSaver::flush(){
    if(!m_inited)return false;
    m_buffer += "\n";
    m_writer.write(m_buffer.c_str(),m_buffer.length());
    m_writer.flush();
    m_buffer = "";
    return true;
}
bool LogSaver::close(){
    if(!m_inited)return false;
    this->flush();
    m_writer.close();
    m_inited = false;
    return true;
}
LogSaver::~LogSaver(){
    ///Close
    this->close();
}
void LogSaver::operator <<(string v){
    if(openedStoring){
        #ifdef LOG_AS_CON
        soutn(v);
        #endif // LOG_AS_CON
        m_buffer += v;
    }
}
void LogSaver::operator <<(int v){(*this) << to_string(v);}
void LogSaver::operator <<(double v){(*this) << to_string(v);}
void LogSaver::operator <<(float v){(*this) << to_string(v);}
void LogSaver::operator <<(char * v){(*this) << string(v);}

CPUInfo::CPUInfo(){
    #ifdef BUILD_ON_WINDOWS
    ///Windows平台下利用WMIC获取,但是会有弹窗QAQ
    this->CpuID = _Windows_getCPUInfo();
    #endif // BUILD_ON_WINDOWS
}
