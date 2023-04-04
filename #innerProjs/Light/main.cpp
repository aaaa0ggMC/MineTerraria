#include <iostream>
#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include <fstream>

using namespace std;
using namespace sf;

int main()
{
    sf::RenderWindow win(VideoMode(800,600),"Test Light");
    RectangleShape rs = RectangleShape({300,300});
    rs.setPosition(200,200);
    Shader s;
    s.loadFromFile("vert.glsl","frag.glsl");
    //win.setActive();
    while(win.isOpen()){
        sf::Event e;
        while(win.pollEvent(e)){
            if(e.type == e.Closed)win.close();
        }
        win.clear();
        win.draw(rs,&s);
        win.display();
    }
    return 0;
}
