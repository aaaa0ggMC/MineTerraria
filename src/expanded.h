#ifndef EXPANDED_H_INCLUDED
#define EXPANDED_H_INCLUDE
#include <stdlib.h>
#include <string>
#include <vector>
#include <windows.h>
#include <psapi.h>
#include <process.h>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "debugIO.h"

using namespace std;
using namespace sf;

#ifdef ENABLE_FPS
#define showFpsDB FPS_Regular
#else
#define showFpsDB
#endif // ENABLE_FPS

///From kernel.h
#ifdef MODE_DEBUG
#define FPS_Regular {static cck::Clock averTimer;static cck::Clock fpsTimer;static double aver = 0;static double fpsAll = 0;static double splitT = 0;static Text fpsT("fps:detecting\nmpf:detecting",*dfont,16);fpsT.setOutlineColor(Color::Black);fpsT.setOutlineThickness(1);double eq = (double)fpsTimer.GetOffset();\
splitT += eq;if(eq != 0 && splitT > FPerS(UPDATE_FPS_PER_SEC)){\
splitT = 0;double fps = (double)1000 / eq;fpsAll += fps;if(averTimer.Now().offset >= 1500){averTimer.GetOffset();if(aver == 0){aver = fpsAll / (4*UPDATE_FPS_PER_SEC);}else{aver = (aver + fpsAll / (1.5*UPDATE_FPS_PER_SEC))/2;}fpsAll = 0;}fpsT.setFillColor(Color::Yellow);\
fpsT.setString("fps:" + to_string((int)fps) + "\nmpf:" + to_string(eq) + "\nAver:" + to_string((int)aver));}window.draw(fpsT);}
#else
#define FPS_Regular {static cck::Clock fpsTimer;static double splitT = 0;static Text fpsT("fps:detecting",*dfont,16);fpsT.setOutlineColor(Color::Black);fpsT.setOutlineThickness(1);double eq = (double)fpsTimer.GetOffset();\
splitT += eq;if(eq != 0 && splitT > FPerS(UPDATE_FPS_PER_SEC)){\
splitT = 0;double fps = (double)1000 / eq;fpsT.setFillColor(Color::Yellow);\
fpsT.setString("fps:" + to_string((int)fps));}window.draw(fpsT);}
#endif // MODE_DEBUG

//Smaller than 0 means no restrict
#define RESTRICT_FRAME_LIMIT -1
#define UPDATE_FPS_PER_SEC 2

#define timeGt "[" + getTime() + "]:"

//Game Controls
#define SUN_MOON_RT checkDebug(0.1,0.01)
#define BACK_MOVE checkDebug(1.0,0.1)

#define ONLY_INIT_ONCE_INIT  static bool inited_oios = false
#define ONLY_INIT_ONCE_START if(!inited_oios){inited_oios = true;
#define ONLY_INIT_ONCE_END }

#define ONLY_INIT_ONCE_INIT_I(i)  static bool inited_oios##i = false
#define ONLY_INIT_ONCE_START_I(i) if(!inited_oios##i){inited_oios##i = true;
#define EASY_LOG(h,s,o) sl(mo,"\n-"+ h + "----Log Start---------------------\n" + s + "\n--------------Log End-----------------\n");

//Lamda to one!!!
#define ml(x,m) ([&](void)->auto{auto v = x;m;return x;})()

struct GameSceneContacting{
private:
    bool m_boolInit;
    int m_boolSize;
public:
    vector<bool> bools;

    GameSceneContacting(int boolSize = 0,bool boolInit = false){
        m_boolInit = boolInit;
        m_boolSize = boolSize;
    }

    void clear(){
        bools.resize(m_boolSize,m_boolInit);
    }
};

struct LoadingProgress{
    string nowLoading;
    float nowProg;
    float allProg;
    bool isCritical;
    bool invokedError;
    bool loadKerFail;
    string singalError;
    string finalError;

    LoadingProgress():nowLoading(""),nowProg(0),
    allProg(0),invokedError(false),singalError(""),finalError(""){
        isCritical = false;
        loadKerFail = false;
    }
};

class LogSaver{
private:
    string m_buffer;
    bool m_inited;
    ofstream m_writer;
    bool openedStoring;
public:
    LogSaver(){
        m_inited = false;
        m_buffer = "";
        openedStoring = true;
    }
    ~LogSaver();
    bool initStoring(string storeIn);
    bool flush();
    bool close();
    void operator <<(string v);
    void operator <<(int v);
    void operator <<(double v);
    void operator <<(float v);
    void operator <<(char * v);
    bool getStatus(){return m_inited;}
    void closeStoring(){openedStoring = false;}
};

struct MemTp{float mem;float vmem;};
struct GlMem{float percent;float phy;float vir;float usephy;};

struct CPUInfo{
    string CpuID;
    CPUInfo();
};



string _Windows_getCPUInfo();
MemTp GetCurrentMemoryUsage();
GlMem GetGlobalMemoryUsage();
string translateSeconds(int msecs);

#endif // EXPANDED_H_INCLUDED
