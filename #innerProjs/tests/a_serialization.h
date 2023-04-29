#ifndef A_SERILIZATION_H_INCLUDED
#define A_SERILIZATION_H_INCLUDED
#include <string>
#include <vector>

using namespace std;

#define SIMP_DEF_SERI(tp) void Serialize(tp)
#define SERI_PTR(tp) void Serialize(tp*,size_t)

namespace alib{
    class Serializer{
        template<class T> void Serialize(T);
        template<class T> void Serialize(T,size_t);

        template<class T> T UnSerialize();
    };
}

/*
SIMP_DEF_SERI(int);
SIMP_DEF_SERI(double);
SIMP_DEF_SERI(float);
SIMP_DEF_SERI(char);
SIMP_DEF_SERI(short);
SIMP_DEF_SERI(long);
SIMP_DEF_SERI(unsigned int);
SIMP_DEF_SERI(unsigned long);
SERI_PTR(char);
SERI_PTR(int);
SERI_PTR(float);
SERI_PTR(double);
SERI_PTR(long);
SERI_PTR(short);
SERI_PTR(unsigned int);
SERI_PTR(unsigned long);
void Serialize(string);
*/

#endif // A_SERILIZATION_H_INCLUDED
