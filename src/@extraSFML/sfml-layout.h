#ifndef SFML_LAYOUT_H_INCLUDED
#define SFML_LAYOUT_H_INCLUDED
#include "../kernel.h"

using namespace sf;
using namespace std;

enum PosRelative{
    PosCenter,
    PosLeftUp,
    PosRightDown,
    PosPercent,
    PosNotChange,
    PosLeftUpShow,//Assure to show object
    PosRightDownShow
};


//Hor横向方向 ver竖直方向
Vector2f setPosRelative(FloatRect rct,Vector2i winsz,PosRelative hor,PosRelative ver,float horPer = 0,float verPer = 0);
Vector2f appendPixel(FloatRect rct,float xm,float ym);

typedef int(*RunFn)(Text *,void *);

class LayoutController{
public:
    vector<Text*> texts;
    vector<RunFn> fns;
    vector<Vector2f> poses;
    float x,y;
    int align,padding;
    Text& operator[](unsigned int index){
        return *texts[index];
    }
    LayoutController* Move(float x,float y){
        this->x += x;
        this->y += y;
        return this;
    }
    LayoutController* Set(float xx = 0,float yy = 0){
        x = xx;
        y = yy;
        return this;
    }

    LayoutController():align(0),padding(0){Set();}

    void Append(Text * t,RunFn op = NULL){
        texts.push_back(t);
        fns.push_back(op);
    }

    void RunOnClick(unsigned int index,void * v){
        if(index >= texts.size())return;
        if(fns[index])fns[index](texts[index],v);
    }

    void DynamicUpdate(){
        poses.clear();
        for(Text * t : texts){
            poses.push_back(t->getPosition());
            t->setPosition(t->getPosition() + Vector2f(x,y));
        }
    }

    #define ORI_XCENTER 0x00000001
    #define ORI_YCENTER 0x00000010
    #define ORI_XLEFT   0x00000100
    #define ORI_XRIGHT  0x00001000
    #define ORI_YTOP    0x00010000
    #define ORI_YBOTTOM 0x00100000
    #define ORI_CENTER (ORI_XCENTER | ORI_YCENTER)

    void draw(RenderTarget & r){
        for(Text*t : texts){
            r.draw(*t);
        }
    }

    #define UNREACHABLE 1145141919

    unsigned int PositionDetects(Vector2f in){
        for(unsigned int i = 0;i < texts.size();++i){
            if(texts[i]->getGlobalBounds().contains(in)){
                return i;
            }
        }
        return UNREACHABLE;
    }

    template<class T> unsigned int PositionDetects(Vector2f in,T fn){
        for(unsigned int i = 0;i < texts.size();++i){
            if(texts[i]->getGlobalBounds().contains(in)){
                fn(texts[i],true);
                return i;
            }
            fn(texts[i],false);
        }
        return UNREACHABLE;
    }


    ///recv only:ORI_CENTER,ORI_XLEFT Now
    LayoutController* SetTextsAlign(int i){
        align = i;
        return this;
    }

    LayoutController* SetTextPadding(int padding){
        this->padding = padding;
        return this;
    }

    void StaticForm(float xStart=0,float yStart=0,float xend=1024,float yend=1024){
        float appy = yStart;
        float w = xend-xStart;
        for(Text * t : texts){
            if(align == ORI_CENTER){
                    t->setPosition(Vector2f((w - (t->getGlobalBounds().width))/2,appy));
            }else t->setPosition(Vector2f(xStart,appy));
            appy += t->getGlobalBounds().height + padding;
        }
    }

    ~LayoutController(){
        for(Text * t : texts){
            if(t)delete t;
        }
    }
};

#endif // SFML_LAYOUT_H_INCLUDED
