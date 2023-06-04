#define SIMPLE_SPD
#include "spdlog.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <malloc.h>
#include "aaa_util.h"

//using namespace spdlog;
using namespace std;
using namespace alib;

LogSaver * LogSaver::instance;

bool alib::LogSaver::SetFileOutput(string path){
    ofs.open(path,ios::out | ios::trunc);
    m_inited = ofs.good();
    if(buffer.compare("")){
        ofs << buffer;
        buffer = "";
    }
    return m_inited;
}
void alib::LogSaver::flush(){
    if(!m_inited)return;
    ofs.flush();
}
void alib::LogSaver::close(){
    if(!m_inited)return;
    this->flush();
    m_inited = false;
}

alib::LogSaver::~LogSaver(){
    ///Close
    this->close();
    DeleteCriticalSection(&cs);
}

LogSaver::LogSaver(bool otc,bool v,int lg){
    m_inited = false;
    if(v && (instance == NULL)){
        instance = this;
    }
    output2c = otc;
    mode = LOG_SH_BASIC;
    buffer = "";
    showLogs(lg);
    InitializeCriticalSection(&cs);
}

LogSaver* LogSaver::get(){
    return instance;
}

void LogSaver::set(LogSaver* l){
    instance = l;
}

void LogSaver::OutputToConsole(bool value){
    this->output2c = value;
}

bool LogSaver::getStatus(){
    return m_inited;
}

IData LogSaver::genType(int l){
    if(l&LOG_INFO)return {"[INFO]",0x0E};
    else if(l&LOG_CRITI)return {"[CRITICAL]",0x4f};
    else if(l&LOG_WARN)return {"[WARN]",0x09};
    else if(l&LOG_DEBUG)return {"[DEBUG]",0x0f};
    else if(l&LOG_ERROR)return {"[ERROR]",0xf4};
    else if(l&LOG_TRACE)return {"[TRACE]",0x0f};
    else return {"",0};
}

int LogSaver::getMode(){return mode;}

int LogSaver::getMask(){return showlg;}

CriticalLock::CriticalLock(CRITICAL_SECTION & c){
    cs = &c;
    EnterCriticalSection(cs);
}

CriticalLock::~CriticalLock(){
    LeaveCriticalSection(cs);
}

void LogSaver::log(int level,string& msg,string &head){
    if(level & LOG_OFF || !(showlg & level))return;
    IData ist = genType(level);
    string strd = "";
    CriticalLock lock(cs);
    if(mode & LOG_SHOW_TIME){
        strd += string("[") + Util::getTime() + "]";
        if(output2c)cout << strd;
    }
    if(mode & LOG_SHOW_TYPE){
        if(output2c)Util::colorfulPrint(ist.str,ist.color);
        strd += ist.str;
    }
    string restOut = "";
    if(mode & LOG_SHOW_HEAD && head.compare("")){
        restOut += string("[") + head + "]";
    }
    if(mode & LOG_SHOW_ELAP){
        restOut += string("[") + to_string(clk.Now().offset) + " ms]";
    }
    if(mode & LOG_SHOW_PROC){
        restOut += string("[PID:") + to_string(GetCurrentProcessId()) + "]";
    }
    if(mode & LOG_SHOW_THID){
        restOut += string("[TID:") + to_string(GetCurrentThreadId()) + "]";
    }
    restOut += string(":") + msg + "\n";
    strd += restOut;
    if(output2c)cout << restOut;
    if(m_inited){
        ofs << strd;
    }else{
        buffer += strd;
    }
}

void LogSaver::configure(int mode){this->mode = mode;}

void LogSaver::showLogs(int logs){showlg = logs;}

string LogSaver::makeMsg(int level,string & msg,string &head,bool ends){
    IData ist = genType(level);
    string rout = "";
    if(mode & LOG_SHOW_TIME){
        rout += string("[") + Util::getTime() + "]";
    }
    if(mode & LOG_SHOW_TYPE){
        rout += ist.str;
    }
    if(mode & LOG_SHOW_HEAD && head.compare("")){
        rout += string("[") + head + "]";
    }
    if(mode & LOG_SHOW_ELAP){
        rout += string("[") + to_string(clk.Now().offset) + " ms]";
    }
    if(mode & LOG_SHOW_PROC){
        rout += string("[PID:") + to_string(GetCurrentProcessId()) + "]";
    }
    if(mode & LOG_SHOW_THID){
        rout += string("[TID:") + to_string(GetCurrentThreadId()) + "]";
    }
    rout += string(":") + msg + (ends?"\n":"");
    return rout;
}

void LogFactory::info(string msg){if(i)i->log(LOG_INFO,msg,head);}
void LogFactory::error(string msg){if(i)i->log(LOG_ERROR,msg,head);}
void LogFactory::critical(string msg){if(i)i->log(LOG_CRITI,msg,head);}
void LogFactory::debug(string msg){if(i)i->log(LOG_DEBUG,msg,head);}
void LogFactory::trace(string msg){if(i)i->log(LOG_TRACE,msg,head);}
void LogFactory::warn(string msg){if(i)i->log(LOG_WARN,msg,head);}

LogFactory::LogFactory(string a,bool b,LogSaver * c){
    head = a;
    if(b){
        i = LogSaver::instance;
    }else i = c;
}

void LogFactory::log(int l,string m){if(i)i->log(l,m,head);}
