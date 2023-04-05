#include <iostream>
#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include <fstream>
#include <math.h>

using namespace std;
using namespace sf;

struct Light{
    Vector2f position;
    Color color;
    float range;
};

using Triangle = vector<Vector2f>;
using Line = vector<Vector2f>;

void operator +=(Triangle & tri,Vector2f a){
    tri[0] += a;
    tri[1] += a;
    tri[2] += a;
}

void operator -=(Triangle & tri,Vector2f a){
    tri[0] -= a;
    tri[1] -= a;
    tri[2] -= a;
}

Vector2f operator *(Vector2f v,float c){
    return Vector2f(v.x * c,v.y * c);
}
Vector2f operator *(float c,Vector2f v){
    return Vector2f(v.x * c,v.y * c);
}

Light l = {{400,300},{255,255,255},200};

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

struct AngleData{
    float angle;
    float depth;
};

#define PI 3.1415926
#define TIAO 6.2831852
#define PIECES 360

[[nodiscard]] Texture * GenShadowMap(Light & l,vector<Triangle> triangles,vector<Line> & stls){
    vector<AngleData> angleData;
    ///计算出相对于光的三角阵列
    for(Triangle & i : triangles){
        i -= l.position;

        vector<float> maxDot;
        ///确立每个三角形中点积最大的点
        {
            float dotv = dot(normalize(i[0]),normalize(i[1]));
            float rdotv = dot(normalize(i[1]),normalize(i[2]));
            maxDot = {0,1,2,dotv};
            if(rdotv < dotv){
                maxDot = {1,2,0,rdotv};
            }
            dotv = dot(normalize(i[2]),normalize(i[0]));
            if(dotv < maxDot[3]){
                maxDot = {0,2,1,dotv};
            }
        }
        vector<Line> lines;
        ///确定绘制选段（有关的线段并存储）
        if(dot(i[(int)maxDot[2]],i[(int)maxDot[0]] - i[(int)maxDot[2]]) > 0){
            ///这时，光照到的线段有两段
            lines.push_back({i[(int)maxDot[0]],i[(int)maxDot[2]] - i[(int)maxDot[0]]});
            lines.push_back({i[(int)maxDot[1]],i[(int)maxDot[2]] - i[(int)maxDot[1]]});
        }else{
            ///这时，只会找到一条线段
            lines.push_back({i[(int)maxDot[0]],i[(int)maxDot[1]] - i[(int)maxDot[0]]});
        }
        stls = lines;
        for(Line & ln : lines){
            ///进行线段均深度采样
            for(float t=0;t <= 1;t += 1/SAMPLE_C){
                ///计算和向量
                Vector2f sum = ln[0] + (t * ln[1]);
                ///计算阴影贴图角度
                //cout << dot(normalize(sum),normal) << endl;
                float theta = acos(normalize(sum).x);
                if(sum.y < 0)theta = TIAO - theta;
                ///存储
                angleData.push_back({theta,length(sum)});
            }
        }
    }

    ///逐角度预估并生成1D贴图
    Uint8 * data = new Uint8[PIECES * 4];
    Texture * t = new Texture();
    t->create(PIECES,1);
    for(unsigned int pp = 0;pp < PIECES;++pp){

        float ang = ((float)pp)/PIECES * TIAO;
        data[pp*4 + 0] = 255;
        data[pp*4 + 1] = 0;
        data[pp*4 + 2] = 0;
        data[pp*4 + 3] = 255;
        for(AngleData& ag : angleData){
            if(abs(ag.angle - ang) <= 360.0/PIECES){
                ///谨防溢出
                unsigned int v = ag.depth / l.range * 255;
                data[pp*4 + 0] = v>255?255:v;
                break;
            }
        }
        /*
        data[pp*4 + 0] = 255;
        data[pp*4 + 1] = 0;
        data[pp*4 + 2] = 0;
        data[pp*4 + 3] = 255;
        */
    }
    t->update(data);
    delete data;
    ///返回Texture指针
    return t;
}

int main()
{
    sf::RenderWindow win(VideoMode(800,600),"Test Light");
    RenderTexture shaMul;
    shaMul.create(win.getSize().x,win.getSize().y);
    RectangleShape rs = RectangleShape({300,300});
    rs.setFillColor({100,100,0});
    rs.setOutlineThickness(10);
    rs.setOutlineColor({0,0,0});
    rs.setPosition(200,200);
    Triangle tri = {{100,100},{200,200},{100,400}};
    VertexArray v(Triangles,3);
    VertexArray ls(sf::Lines,8);
    v[0].position = tri[0];
    v[1].position = tri[1];
    v[2].position = tri[2];
    v[0].color = Color(255,255,255);
    v[1].color = Color(255,255,255);
    v[2].color = Color(255,255,255);
    vector<Line> lss;
    Texture * shadowMap = GenShadowMap(l,{tri},lss);
    ls[0].color = ls[1].color = ls[2].color = ls[3].color = Color::Red;
    ls[4].color = ls[5].color = ls[6].color = ls[7].color = Color::Red;
    Sprite sp(*shadowMap);
    sp.scale(0.2,100);
    RectangleShape circ({2*l.range,2*l.range});
    circ.setPosition(l.position - Vector2f(l.range,l.range));
    circ.setFillColor(Color::White);
    Shader s;
    Color globalLC = Color(0,0,0);
    s.loadFromFile("vert.glsl","frag.glsl");
    //s.loadFromFile("vert.glsl",s.Vertex);
    s.setUniform("l.color",(Glsl::Vec4)l.color);
    s.setUniform("l.position",l.position);
    s.setUniform("l.range",l.range);
    s.setUniform("shTex",*shadowMap);
    s.setUniform("globalLightColor",(Glsl::Vec4)globalLC);
    s.setUniform("globalMul",1.0f);
    RenderStates rens = rens.Default;
    rens.blendMode = sf::BlendMultiply;
    //win.setActive();
    while(win.isOpen()){
        sf::Event e;
        while(win.pollEvent(e)){
            if(e.type == e.Closed)win.close();
        }
        if(Keyboard::isKeyPressed(Keyboard::W)){
            l.position.y -= 1;
            circ.setPosition(l.position - Vector2f(l.range,l.range));
            delete shadowMap;
            shadowMap = GenShadowMap(l,{tri},lss);
        }else if(Keyboard::isKeyPressed(Keyboard::A)){
            l.position.x -= 1;
            circ.setPosition(l.position - Vector2f(l.range,l.range));
            delete shadowMap;
            shadowMap = GenShadowMap(l,{tri},lss);
        }else if(Keyboard::isKeyPressed(Keyboard::S)){
            l.position.y += 1;
            circ.setPosition(l.position - Vector2f(l.range,l.range));
            delete shadowMap;
            shadowMap = GenShadowMap(l,{tri},lss);
        }else if(Keyboard::isKeyPressed(Keyboard::D)){
            l.position.x += 1;
            circ.setPosition(l.position - Vector2f(l.range,l.range));
            delete shadowMap;
            shadowMap = GenShadowMap(l,{tri},lss);
        }
        ls[0].position = l.position;
        ls[1].position = lss[0][0] + l.position;
        ls[2].position = lss[0][0] + l.position;
        ls[3].position = lss[0][0] + l.position + lss[0][1];
        if(lss.size()>1){
            ls[4].position = l.position;
            ls[5].position = lss[1][0] + l.position;
            ls[6].position = lss[1][0] + l.position;
            ls[7].position = lss[1][0] + l.position + lss[1][1];
        }else{
            ls[4].position = {0,0};
            ls[5].position = {0,0};
            ls[6].position = {0,0};
            ls[7].position = {0,0};
        }
        win.clear(Color(255,255,255));
        //win.draw(rs);
        shaMul.clear(globalLC);
        shaMul.draw(circ,&s);
        shaMul.display();
        Sprite lp(shaMul.getTexture());
        win.draw(lp,rens);
        win.draw(v);
        win.draw(sp);
        win.draw(ls);
        win.display();
    }
    return 0;
}
