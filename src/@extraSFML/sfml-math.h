#ifndef SFML_MATH_H_INCLUDED
#define SFML_MATH_H_INCLUDED
#include <math.h>
#include <SFML/System.hpp>

using namespace sf;
using namespace std;

#define PI 3.14159265
#define deg2rad(deg) (deg)*PI/180
#define rad2deg(rad) (rad)*180/PI

Vector2f RotateAround(float ang,float len,Vector2f center);
Vector2f RotateAround(float ang,Vector2f Ori,Vector2f center);
Vector2f operator+(Vector2f a,Vector2f b);
Vector2f operator-(Vector2f a,Vector2f b);
Vector2f operator*(Vector2f a,float b);
Vector2f operator/(Vector2f a,float b);
Vector2f operator*(float b,Vector2f a);
Vector2f operator/(float b,Vector2f a);

inline float Dot(Vector2f a,Vector2f b){
    return a.x * b.x + a.y * b.y;
}
inline float Length(Vector2f a){
    return sqrt(a.x * a.x + a.y * a.y);
}
inline Vector2f Normalize(Vector2f a){
    float l = Length(a);
    return Vector2f(a.x/l,a.y/l);
}
inline float getAngle(Vector2f a,Vector2f b){
    return rad2deg(acos(Dot(Normalize(a),Normalize(b))));
}
inline float getAngleSigned(Vector2f a,Vector2f b){
    float rtv = getAngle(a,b);
    if(a.x >= b.x)return rtv;
    else return -rtv;
}

template<class T,class T2> float Distance(Vector2<T> t1,Vector2<T2> t2){
    float x = t1.x - t2.x;
    float y = t1.y - t2.y;
    return Length(Vector2f(x,y));
}
#endif // SFML_MATH_H_INCLUDED
