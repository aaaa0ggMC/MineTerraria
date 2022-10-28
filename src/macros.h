#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED

#define randRange(minEq,maxEq) (rand()%((maxEq)+1 - (minEq)) + (minEq))

#define invokeAnErrorEx(lp,msg) lp->singalError = (msg);lp->finalError += lp->singalError + "\n";lp->invokedError = true
#define invokeAnError(msg) invokeAnErrorEx(lp,msg)

#define waitingInvokingEx(lp) while(lp->invokedError)Sleep(500)
#define waitingInvoking waitingInvokingEx(lp)

#define invokeOneEx(lp,msg) invokeAnErrorEx(lp,msg);waitingInvokingEx(lp)
#define invokeOne(msg) invokeAnError(msg);waitingInvoking

#define setStrEx(lp,msg) lp->nowLoading = (msg)
#define setStr(msg) setStrEx(lp,msg)

#define setProgEx(lp,num) lp->nowProg = (num)
#define setProg(num) setProgEx(lp,num)

#define setAProgEx(lp,num) lp->allProg = (num)
#define setAProg(num) setAProgEx(lp,num);

#define getProgEx(lp) lp->nowProg
#define getProg getProgEx(lp)

#define getAProgEx(lp) lp->allProg
#define getAProg getAProgEx(lp)

#define addProgEx(lp,n) lp->nowProg += n
#define addProg addProgEx(lp,n)

#define addAProgEx(lp,n) lp->allProg += n
#define addAProg(n) addAProgEx(lp,n)

#define init4of2(a,b,c,d,func,va,vb,vc,vd) {a.func(va,vb);b.func(vc,vd);c.func(vc,vd);d.func(vc,vd);}
#define init41of2(a,b,c,d,func,va,vc) {a.func(va);b.func(vc);c.func(vc);d.func(vc);}
#define init4(a,b,c,d,func,va,vb) {a.func(va,vb);b.func(va,vb);c.func(va,vb);d.func(va,vb);}
#define init41(a,b,c,d,func,va) {a.func(va);b.func(va);c.func(va);d.func(va);}

#define initEPI static bool epi_inited = false
#define enterPerInit if(!epi_inited){epi_inited = true;
#define endEPI }
#define ReInitEPI epi_inited = false

#endif // MACROS_H_INCLUDED
