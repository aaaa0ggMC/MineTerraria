#ifndef DEBUGIO_H_INCLUDED
#define DEBUGIO_H_INCLUDED

#define out(A) cout << A
#define outn(A) cout << A << endl
#define el endl
#define outv(A) outn(string(#A ":(") << (A).x << "," << (A).y << ")")
#define outs(A) outn(#A << ":\n" << A)
#define checkDebug(yes,no) yes
#define sAlive outn("I am alive!" << __FILE__ << " " <<  __LINE__)

#define _T TEXT

#define EAssert(s) MessageBox(NULL,s,_T("UnlimitedLife Error"),MB_OK | MB_ICONERROR | MB_TOPMOST);
#define EAssertEx(h,s) MessageBox(h,s,_T("UnlimitedLife Error"),MB_OK | MB_ICONERROR | MB_TOPMOST);

#define EAssertExEx(h,s,t) MessageBox(h,_T(s),_T(t),MB_OK | MB_ICONERROR | MB_TOPMOST);

#define block(x)

#endif // DEBUGIO_H_INCLUDED
