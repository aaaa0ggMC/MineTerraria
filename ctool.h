//ctool.h debug中的fileIO::file_size出自其中，是个c++工具包，需要的话自取
#ifndef CTOOL_INC
#define CTOOL_INC
#include <windows.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <math.h>
#include "CClock.h"

using namespace std;

#ifdef BUILD_DEBUG
#define O(x) cout << x
#define Oln(x) cout << x << endl
#endif // BUILD_DEBUG

namespace strps{
    string GetTranslateString(string in);
    void split(vector<string> & vct,const string & line,const char sep);
    string Trim(string & str);
    void Stringsplit(string str, string splits, vector<string>& res);
    namespace encoding{
        string GBKToUTF8(const string &strGBK);
        string UTF8ToGBK(const string &strUTF8);
    }
}

namespace fileIO{
    int file_size(char* filename);
    bool check_exists(char* filename);
    string readAll(ifstream & reader);
    int writeAll(string fth,string s);
}

namespace num{
    namespace random{
        typedef struct RandomShakeStruct{
            float shakeValue;
            float time;
            float timePerRound;
            float mn;
            float mx;
            float start;
            int stDirection;
            cck::Clock insideClock;
        } ShakeSt;
        void ShakeInit(ShakeSt & st,float timePerRound,float minv,float maxv,float start,int startDiection = 1);
        //抖动式随机数，目前为beta状态
        void Shake(ShakeSt & st);
    }
    namespace vectors{
        //向量
        struct Vector{
            float x;
            float y;
            float z;
            Vector(float v0,float v1,float v2 = 0);
        };
        Vector Normalize(Vector);
    }
}

#endif // CTOOL_INC
