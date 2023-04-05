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
    int AppendPlay(string filePath,bool keep = true,string given = "",bool loop = false){
        Music * m = new Music();
        if(!m->openFromFile(filePath)){
            delete m;
            return -1;
        }
        m->setLoop(loop);
        holded.push_back({given,filePath,m,loop,keep});
        m->play();
        return 0;
    }

    void Update(){
        vector<MusicStruct> m_in;
        for(const auto & i : holded){
            if(!i.loop && !i.keep && i.music->getStatus() == Music::Stopped){
                delete i.music;
            }else{
                m_in.push_back(i);
            }
        }
        holded = m_in;
    }

    void PauseAll(){
        for(const auto & i : holded){
            i.music->pause();
        }
    }


    void ResumeAll(){
        for(const auto & i : holded){
            i.music->play();
        }
    }

    ~MusicController(){
        for(auto & i : holded)delete i.music;
    }
    Music* operator[](string id){
        for(const auto & i : holded){
            if(!i.givenId.compare(id))return i.music;
        }
        return NULL;
    }
};

#endif // MUSICCONTROLLER_H_INCLUDED
