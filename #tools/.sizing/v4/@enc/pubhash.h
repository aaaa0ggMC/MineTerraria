#ifndef PUBHASH_H_INCLUDED
#define PUBHASH_H_INCLUDED

#include <string>

using namespace std;

unsigned int HashStringToUInt(string str,bool tryHashNumber = true,unsigned int blendMask = 10471794);

#endif // PUBHASH_H_INCLUDED
