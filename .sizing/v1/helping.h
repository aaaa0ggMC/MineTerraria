#ifndef HELPING_H_INCLUDED
#define HELPING_H_INCLUDED
#include "kernel.h"

using namespace std;

#define FPS_Regular {static cck::Clock fpsTimer;static float splitT = 0;static Text fpsT("fps:detecting",*dfont,16);float eq = (float)fpsTimer.GetOffset();\
splitT += eq;if(eq != 0 && splitT > FPerS(UPDATE_FPS_PER_SEC)){\
splitT = 0;float fps = (float)1000 / eq;fpsT.setFillColor(Color::Yellow);\
fpsT.setString("fps:" + to_string(fps));}window.draw(fpsT);}

#ifdef ENABLE_FPS
#define showFpsDB FPS_Regular
#else
#define showFpsDB
#endif // ENABLE_FPS

#define FPerS(tm) (1000/tm)

#define crange(v,min,max) ((v) > (min) && (v) < (max))
#define crangeEq(v,emin,emax) ((v) >= (emin) && (v) <= (emax))
#define percentage(now,min,max) (((now) >= (max))? 1 : ((float)(now)/(float)((max)-(min))))

///Mouse
#define MS_DOWN false
#define MS_NDOWN true

template<class K,class V> int MapCheckKey(const map<K,V> & mapA,const K & key);//Returns 1 when has member
string getTime();
void getFileNames(string path, vector<string>& files);

#endif // HELPING_H_INCLUDED
