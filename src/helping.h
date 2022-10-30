#ifndef HELPING_H_INCLUDED
#define HELPING_H_INCLUDED
#include "kernel.h"

using namespace std;

#ifdef MODE_DEBUG
#define FPS_Regular {static cck::Clock averTimer;static cck::Clock fpsTimer;static double aver = 0;static double fpsAll = 0;static double splitT = 0;static Text fpsT("fps:detecting\nmpf:detecting",*dfont,16);double eq = (double)fpsTimer.GetOffset();\
splitT += eq;if(eq != 0 && splitT > FPerS(UPDATE_FPS_PER_SEC)){\
splitT = 0;double fps = (double)1000 / eq;fpsAll += fps;if(averTimer.Now().offset >= 1500){averTimer.GetOffset();if(aver == 0){aver = fpsAll / (4*UPDATE_FPS_PER_SEC);}else{aver = (aver + fpsAll / (1.5*UPDATE_FPS_PER_SEC))/2;}fpsAll = 0;}fpsT.setFillColor(Color::Yellow);\
fpsT.setString("fps:" + to_string((int)fps) + "\nmpf:" + to_string(eq) + "\nAver:" + to_string((int)aver));}window.draw(fpsT);}
#else
#define FPS_Regular {static cck::Clock fpsTimer;static double splitT = 0;static Text fpsT("fps:detecting",*dfont,16);double eq = (double)fpsTimer.GetOffset();\
splitT += eq;if(eq != 0 && splitT > FPerS(UPDATE_FPS_PER_SEC)){\
splitT = 0;double fps = (double)1000 / eq;fpsT.setFillColor(Color::Yellow);\
fpsT.setString("fps:" + to_string((int)fps));}window.draw(fpsT);}
#endif // MODE_DEBUG

#ifdef ENABLE_FPS
#define showFpsDB FPS_Regular
#else
#define showFpsDB
#endif // ENABLE_FPS

#define FPerS(tm) (1000/tm)

#define crange(v,min,max) ((v) > (min) && (v) < (max))
#define crangeEq(v,emin,emax) ((v) >= (emin) && (v) <= (emax))
#define percentage(now,min,max) (((now) >= (max))? 1 : ((float)(now)/(float)((max)-(min))))
#define getPercentage(per,min,max) ((per)*((float)((max) - (min))) + (min))


///Mouse
#define MS_DOWN false
#define MS_NDOWN true

///More safe controlling pointers
#define SafeControlPointer(p,v) if(!p){*p=v;}
#define _SCP SafeControlPointer

#define SafeGetPointerV(p) ((p)?(*p):0)
#define _SGTV SafeGetPointerV


#define _p(x) (((char *)(x) == NULL) ? "[Failed to get]" : (char*)(x))
#define _s(x) (std::string(_p(x)))
#define _f(x) ((float)(x))
#define _d(x) ((double)(x))

#define loop(t) for(unsigned int __loop = 0;__loop < (unsigned int)t;++__loop)
#define loopi(t) for(int _loop = 0;_loop < (int)t;++__loop)
#define loopv(v,t) for(int v = 0;v < (int)t;++ v)

template<class K,class V> int MapCheckKey(const map<K,V> & mapA,const K & key);//Returns 1 when has member
string getTime();
void getFileNames(string path, vector<string>& files);

#endif // HELPING_H_INCLUDED
