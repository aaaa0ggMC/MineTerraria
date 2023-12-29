#include "MusicController.h"

using namespace std;
using namespace sf;


int MusicController::AppendPlay(string filePath,bool keep,string given,bool loop){
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

void MusicController::Update(){
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

void MusicController::PauseAll(){
    for(const auto & i : holded){
        i.music->pause();
    }
}

void MusicController::ResumeAll(){
    for(const auto & i : holded){
        i.music->play();
    }
}

MusicController::~MusicController(){
    for(auto & i : holded)delete i.music;
}

Music* MusicController::operator[](string id){
    for(const auto & i : holded){
        if(!i.givenId.compare(id))return i.music;
    }
    return NULL;
}
