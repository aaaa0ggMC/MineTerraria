#ifndef SFML_MATH_H_INCLUDED
#define SFML_MATH_H_INCLUDED
#include "../kernel.h"
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

#endif // SFML_MATH_H_INCLUDED