#ifndef DEBUGIO_H_INCLUDED
#define DEBUGIO_H_INCLUDED
#define _T TEXT

#define EAssert(s) MessageBox(NULL,s,_T("UnlimitedLife Error"),MB_OK | MB_ICONERROR | MB_TOPMOST);
#define EAssertEx(h,s) MessageBox(h,s,_T("UnlimitedLife Error"),MB_OK | MB_ICONERROR | MB_TOPMOST);

#define EAssertExEx(h,s,t) MessageBox(h,_T(s),_T(t),MB_OK | MB_ICONERROR | MB_TOPMOST);

#define block(x)

#endif // DEBUGIO_H_INCLUDED
