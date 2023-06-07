#ifndef EXPANDED_H_INCLUDED
#define EXPANDED_H_INCLUDE
#include <stdlib.h>
#include <string>
#include <vector>
#include <windows.h>
#include <psapi.h>
#include <process.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "debugIO.h"
#include <fstream>
#include <memory>

using namespace sf;

#define LOG_AS_CON

#ifdef ENABLE_FPS
#define showFpsDB FPS_Regular
#else
#define showFpsDB
#endif // ENABLE_FPS

#define FPS_Regular {static cck::Clock averTimer;static cck::Clock fpsTimer;static double aver = 0;static double fpsAll = 0;static double splitT = 0;static Text fpsT("fps:detecting\nmpf:detecting",*dfont,16);fpsT.setOutlineColor(Color::Black);fpsT.setOutlineThickness(1);double eq = (double)fpsTimer.GetOffset();\
splitT += eq;if(eq != 0 && splitT > FPerS(UPDATE_FPS_PER_SEC)){\
splitT = 0;double fps = (double)1000 / eq;fpsAll += fps;if(averTimer.Now().offset >= 1500){averTimer.GetOffset();if(aver == 0){aver = fpsAll / (4*UPDATE_FPS_PER_SEC);}else{aver = (aver + fpsAll / (1.5*UPDATE_FPS_PER_SEC))/2;}fpsAll = 0;}fpsT.setFillColor(Color::Yellow);\
fpsT.setString("fps:" + to_string((int)fps) + "\nmpf:" + to_string(eq) + "\nAver:" + to_string((int)aver));}window.draw(fpsT);}

//Smaller than 0 means no restrict
#define RESTRICT_FRAME_LIMIT -1
#define UPDATE_FPS_PER_SEC 2

//Game Controls
#define SUN_MOON_RT 0.1
#define BACK_MOVE   1.0

#define ONLY_INIT_ONCE_INIT  static bool inited_oios = false
#define ONLY_INIT_ONCE_START if(!inited_oios){inited_oios = true;
#define ONLY_INIT_ONCE_END }

#define ONLY_INIT_ONCE_INIT_I(i)  static bool inited_oios##i = false
#define ONLY_INIT_ONCE_START_I(i) if(!inited_oios##i){inited_oios##i = true;

//Lamda to one!!!
#define ml(x,m) ([&](void)->auto{auto v = x;m;return x;})()

struct GameSceneContacting{
private:
    bool m_boolInit;
    int m_boolSize;
public:
    std::vector<bool> bools;

    GameSceneContacting(int boolSize = 0,bool boolInit = false){
        m_boolInit = boolInit;
        m_boolSize = boolSize;
    }

    void clear(){
        bools.resize(m_boolSize,m_boolInit);
    }
};

struct LoadingProgress{
    std::string nowLoading;
    float nowProg;
    float allProg;
    bool isCritical;
    bool invokedError;
    bool loadKerFail;
    std::string singalError;
    std::string finalError;

    LoadingProgress():nowLoading(""),nowProg(0),
    allProg(0),invokedError(false),singalError(""),finalError(""){
        isCritical = false;
        loadKerFail = false;
    }
};
#endif // EXPANDED_H_INCLUDED