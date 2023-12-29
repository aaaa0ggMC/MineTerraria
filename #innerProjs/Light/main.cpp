#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <GL/gl.h>
#include <fstream>
#include <math.h>
#include "CClock.h"

using namespace std;
using namespace sf;

struct Light{
    Vector2f position;
    Color color;
    float range,loss,multi,floating;
    //RenderTexture*sh;
};

Vector2f operator *(Vector2f v,float c){
    return Vector2f(v.x * c,v.y * c);
}
Vector2f operator *(float c,Vector2f v){
    return Vector2f(v.x * c,v.y * c);
}

Light l = {{400,300},{255,255,255},200,0.97,1,0.02};

#define SAMPLE_C 8.0

inline float dot(Vector2f a,Vector2f b){
    return a.x * b.x + a.y * b.y;
}

inline float length(Vector2f & a){
    return sqrt(a.x*a.x + a.y*a.y);
}

inline Vector2f normalize(const Vector2f & n){
    return (1/length((Vector2f&)n)) * n;
}

#define PI 3.1415926
#define TIAO 6.2831852
#define PIECES 360


#define UPDATE_FPS_PER_SEC 2
#define FPerS(tm) (1000/tm)

#define FPS_Regular {static cck::Clock averTimer;static cck::Clock fpsTimer;static double aver = 0;static double fpsAll = 0;static double splitT = 0;static Text fpsT("fps:detecting\nmpf:detecting",dfont,16);fpsT.setOutlineColor(Color::Black);fpsT.setOutlineThickness(1);double eq = (double)fpsTimer.GetOffset();\
splitT += eq;if(eq != 0 && splitT > FPerS(UPDATE_FPS_PER_SEC)){\
splitT = 0;double fps = (double)1000 / eq;fpsAll += fps;if(averTimer.Now().offset >= 1500){averTimer.GetOffset();if(aver == 0){aver = fpsAll / (4*UPDATE_FPS_PER_SEC);}else{aver = (aver + fpsAll / (1.5*UPDATE_FPS_PER_SEC))/2;}fpsAll = 0;}fpsT.setFillColor(Color::Yellow);\
fpsT.setString("fps:" + to_string((int)fps) + "\nmpf:" + to_string(eq) + "\nAver:" + to_string((int)aver));}win.draw(fpsT);}


Font dfont;

int main()
{
    cck::Clock clm;
    dfont.loadFromFile("default.ttf");
    sf::RenderWindow win(VideoMode(800,600),"Test Light");
    win.setFramerateLimit(60);
    RenderTexture dmp,lr;
    dmp.create(win.getSize().x,win.getSize().y);
    lr.create(win.getSize().x,win.getSize().y);
    RectangleShape rs = RectangleShape({300,300});
    rs.setFillColor({100,100,0});
    rs.setOutlineThickness(10);
    rs.setOutlineColor({0,0,0});
    rs.setPosition(200,200);
    VertexArray v(Triangles,3);
    v[0].position = {100,100};
    v[1].position = {200,200};
    v[2].position = {100,400};
    v[0].color = Color(255,0,0);
    v[1].color = Color(0,255,0);
    v[2].color = Color(0,0,255);
    RectangleShape circ({2*l.range,2*l.range});
    circ.setPosition(l.position - Vector2f(l.range,l.range));
    circ.setFillColor(Color::White);
    Shader s;
    Color globalLC = Color(255,0,0);
    s.loadFromFile("vert.glsl","frag.glsl");
    //s.loadFromFile("vert.glsl",s.Vertex);
    s.setUniform("l.color",(Glsl::Vec4)l.color);
   // s.setUniform("l.position",l.position);
    s.setUniform("l.range",l.range);
    //s.setUniform("shTex",(*l.sh).getTexture());
    s.setUniform("globalLightColor",(Glsl::Vec4)globalLC);
    s.setUniform("globalMul",1.0f);
    s.setUniform("l.loss",l.loss);
    s.setUniform("l.multi",l.multi);
    s.setUniform("l.floating",l.floating);
    RenderStates rens = rens.Default;
    rens.blendMode = sf::BlendMultiply;
    dmp.clear(Color(255,255,255));
    dmp.draw(v);
    dmp.display();
    //win.setActive();
    while(win.isOpen()){
        sf::Event e;
        while(win.pollEvent(e)){
            if(e.type == e.Closed)win.close();
        }
        if(Keyboard::isKeyPressed(Keyboard::W)){
            l.position.y -= 5;
            circ.setPosition(l.position - Vector2f(l.range,l.range));
        }else if(Keyboard::isKeyPressed(Keyboard::A)){
            l.position.x -= 5;
            circ.setPosition(l.position - Vector2f(l.range,l.range));
        }else if(Keyboard::isKeyPressed(Keyboard::S)){
            l.position.y += 5;
            circ.setPosition(l.position - Vector2f(l.range,l.range));
        }else if(Keyboard::isKeyPressed(Keyboard::D)){
            l.position.x += 5;
            circ.setPosition(l.position - Vector2f(l.range,l.range));
        }
        //win.draw(rs);
        s.setUniform("timing",(float)sin(clm.GetALLTime()/100));
        lr.clear(globalLC);
        lr.draw(circ,&s);
        lr.display();
        Sprite lp(dmp.getTexture());
        win.draw(lp);
        lp.setTexture(lr.getTexture());
        win.draw(lp,rens);
        FPS_Regular;
        win.display();
    }
    return 0;
}
