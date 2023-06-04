#ifndef MUSICCONTROLLER_H_INCLUDED
#define MUSICCONTROLLER_H_INCLUDED

#include <SFML/Audio.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace sf;

struct MusicStruct{
    string givenId;
    string fileName;
    Music * music;
    bool loop;
    bool keep;
};

struct MusicController{
    vector<MusicStruct> holded;
    //int SuspendPlay(string filePath,bool keep = true,string given = "",bool loop = false);
    int AppendPlay(string filePath,bool keep = true,string given = "",bool loop = false);
    void Update();
    void PauseAll();
    void ResumeAll();
    ~MusicController();
    Music* operator[](string id);
};

#endif // MUSICCONTROLLER_H_INCLUDED
