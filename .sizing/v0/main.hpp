#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED
///This is for debug
///Very Important!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//Shows The FPS
#define ENABLE_FPS
///Debug end
#include "kernel.h"
#include "mods.h"
#include "helping.h"
#include "extraSFML/sfml-math.h"
#include "extraSFML/sfml-layout.h"
#include "easyXX/easyColor.h"
#include "easyXX/easyEvents.h"
#include "easyXX/easyFont.h"
#include "easyXX/easyTex.h"
#include "macros.h"
#include "scdta.h"
#include "SFML/Audio.hpp"
#include "cppPy/CppPy.h"

#define RESTRICT_FRAME_LIMIT 120
#define UPDATE_FPS_PER_SEC 2

#define timeGt "[" + getTime() + "]:"

#define MENU_MSC_RANGE_MIN 0
#define MENU_MSC_RANGE_MAX 1

//Game Controls
#define SCALING_PERCENT (float)(0.0003)
#define ROTATING_PERCENT (float)(0.03)
#define SUN_MOON_RT checkDebug(0.1,0.01)
#define SC_MPEC 0.03
#define RT_MPEC 6
#define MV_CLD checkDebug(1,0.1)
#define FLEX_SPEED 1.5
#define CLOUD_C 8
#define BACK_MOVE checkDebug(1,0.1)

#define SONG_SPLIT 4

///FLEX_CHOICES
#define FLEX_SUN 1
#define FLEX_MOON 2

using namespace std;
using namespace sf;
using namespace rapidjson;

///Windows
int DrawStates(RenderWindow & window);
int CartoonStartUp(RenderWindow & window);
int loadingProc(RenderWindow & window);
int mainMenu(RenderWindow & window);
int modsWindow(RenderWindow & window);
int modsExtraWindow(RenderWindow & window);

///States
void LoadFonts();
void * loadingState(void * storeIn);
void tryCreateAppData();

struct LoadingProgress{
    string nowLoading;
    float nowProg;
    float allProg;
    bool isCritical;
    bool invokedError;
    bool loadKerFail;
    string singalError;
    string finalError;

    LoadingProgress():nowLoading(""),nowProg(0),
    allProg(0),invokedError(false),singalError(""),finalError(""){
        isCritical = false;
        loadKerFail = false;
    }
};

class LogSaver{
private:
    string m_buffer;
    bool m_inited;
    ofstream m_writer;
    bool openedStoring;
public:
    LogSaver(){
        m_inited = false;
        m_buffer = "";
        openedStoring = true;
    }
    ~LogSaver();
    bool initStoring(string storeIn);
    bool flush();
    bool close();
    void operator <<(string v);
    void operator <<(int v);
    void operator <<(double v);
    void operator <<(float v);
    void operator <<(char * v);
    bool getStatus(){return m_inited;}
    void closeStoring(){openedStoring = false;}
};

string readStatus(int sta){
    switch(sta){
    case ER_JSON_PARSE:
        return "ER_JSON_PARSE:Parse JSON file error";
    case EXECUTE_SUC:
        return "EXECUTE_SUC:Executed successfully";
    case EXECUTE_FAI:
        return "EXECUTE_FAI:Executed fail";
    case ERR_FAIL_COMPILE:
        return "ERR_FAIL_COMPILE:Fail to compile python file";
    case ERR_GET_FUNCTION:
        //Actually,it should be ERR_GET_MEMBER,however,it is widely used now therefor i can't change it
        return "ERR_GET_FUNCTION:Get a member in the module fail";
    case ERR_GET_KERNEL_VAR:
        return "ERR_GET_KERNEL_VAR:Get a kernel variable in the root module fail";
    case ERR_GET_MENU_ITEMS:
        return "ERR_GET_MENU_ITEMS:Fail to get root module's member:mtmod_loader.mlc.menuStrings";
    case ERR_INIT_MOD:
        return "ERR_INIT_MOD:Fail to initialize the mod";
    case ERR_INIT_ROOT_FAIL:
        return "ERR_INIT_ROOT_FAIL:Fail to initialize the root module";
    case ERR_LOAD_KERNEL:
        return "ERR_LOAD_KERNEL:Cannot load the kernel file";
    case ERR_PY_NOT_INIT:
        return "ERR_PY_NOT_INIT:Python have not initialized";
    default:
        return "ERR_UNKNOWN_ERROR_CODE:Unknown error code";
    }
}

#endif // MAIN_HPP_INCLUDED
