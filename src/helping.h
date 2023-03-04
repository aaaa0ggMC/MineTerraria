#ifndef HELPING_H_INCLUDED
#define HELPING_H_INCLUDED
#include "kernel.h"

using namespace std;

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

#define ExtractEvent(X) Event& e = events[he. X]
#define MatchEKey(k) (e.key.code == Keyboard:: k)

template<class K,class V> int MapCheckKey(const map<K,V> & mapA,const K & key);//Returns 1 when has member
string getTime();
void getFileNames(string path, vector<string>& files);

#endif // HELPING_H_INCLUDED
