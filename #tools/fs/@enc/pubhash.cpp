#include "pubhash.h"
#include <stdlib.h>

using namespace std;

unsigned int HashStringToUInt(string str,bool tryHashNumber,unsigned int blendMask){
    if(tryHashNumber){
        char * check;
        unsigned int ret = (unsigned int)strtoul(str.c_str(),&check,10);
        if(*check == '\0'){
            return ret;
        }
    }
    unsigned int ret = 0;
    for(char & s : str){
        ret += (unsigned int)s;
        ret *= 10;
    }
    return ret + blendMask;
}
