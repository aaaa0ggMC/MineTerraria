#define SIMPLE_SPD
#include "spdlog.h"

using namespace spdlog;
using namespace std;
using namespace alib;

void alib::LogSaver::SetFileOutput(string path,string head){
    lgr = spdlog::basic_logger_mt(head,path,true);
    this->setLevel(LOG_INFO);
    m_inited = true;
}
void alib::LogSaver::flush(){
    if(!m_inited)return;
    lgr->flush();
}
void alib::LogSaver::close(){
    if(!m_inited)return;
    this->flush();
    m_inited = false;
}

alib::LogSaver::~LogSaver(){
    ///Close
    this->close();
}

void alib::LogSaver::setLevel(int lv){
    this->lv = spdlog::level::level_enum(lv);
}

void alib::LogSaver::operator <<(string v){
    if(output2c)spdlog::log(lv,v);
    if(openedStoring){
        if(m_inited)lgr->log(lv,v);
    }
}

LogSaver::LogSaver(bool otc){
    m_inited = false;
    openedStoring = true;
    output2c = otc;
    lv = spdlog::level::level_enum::info;
}

void LogSaver::operator <<(int v){(*this) << to_string(v);}
void LogSaver::operator <<(double v){(*this) << to_string(v);}
void LogSaver::operator <<(float v){(*this) << to_string(v);}
void LogSaver::operator <<(const char * v){(*this) << string(v);}

void LogSaver::OutputToConsole(bool value){
    this->output2c = value;
}

void LogSaver::closeStoring(){
    openedStoring = false;
}

bool LogSaver::getStatus(){
    return m_inited;
}
