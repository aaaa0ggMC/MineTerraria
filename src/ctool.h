//ctool.h debug中的fileIO::file_size出自其中，是个c++工具包，需要的话自取
#ifndef CTOOL_INC
#define CTOOL_INC
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <math.h>
#include "CClock.h"

#define O(x) cout << x
#define Oln(x) cout << x << endl

namespace strps{
    std::string GetTranslateString(std::string in);
    void split(std::vector<std::string> & vct,const std::string & line,const char sep);
    std::string Trim(std::string & str);
    void Stringsplit(std::string str,std::string splits,std::vector<std::string>& res);
    namespace encoding{
        std::string GBKToUTF8(const std::string &strGBK);
        std::string UTF8ToGBK(const std::string &strUTF8);
    }
}

namespace fileIO{
    int file_size(char* filename);
    bool check_exists(char* filename);
    std::string readAll(std::ifstream & reader);
    int writeAll(std::string fth,std::string s);
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
