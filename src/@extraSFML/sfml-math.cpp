#include "sfml-math.h"

using namespace sf;

Vector2f RotateAround(float ang,float len,Vector2f center){
    float nx = -sin(ang);
    float ny = cos(ang);
    return Vector2f(nx*len,ny*len) + center;
}
Vector2f RotateAround(float ang,Vector2f ori,Vector2f center){
    float x = ori.x - center.x;
    float y = ori.y - center.y;
    float len = sqrt(x*x + y*y);
    return RotateAround(ang,len,center);
}

Vector2f operator-(Vector2f a,Vector2f b){
    return Vector2f(a.x - b.x,a.y - b.y);
}

Vector2f operator+(Vector2f a,Vector2f b){
    return Vector2f(a.x + b.x,a.y + b.y);
}

Vector2f operator/(Vector2f a,float b){
    return Vector2f(a.x / b,a.y / b);
}

Vector2f operator*(Vector2f a,float b){
    return Vector2f(a.x * b,a.y * b);
}

Vector2f operator/(float b,Vector2f a){
    return Vector2f(a.x / b,a.y / b);
}

Vector2f operator*(float b,Vector2f a){
    return Vector2f(a.x * b,a.y * b);
}
