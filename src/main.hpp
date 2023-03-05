#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#define GLOBAL_GAME_CONFIG_PATH "config.json"

///目前用处不大
#define BUILD_ON_WINDOWS

///This is for debug
///Very Important!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//Shows The FPS
#define ENABLE_FPS

//注释掉就有背景音乐了
//#define NO_AUDIO

#define LOG_AS_CON

///Used 4 debug
#define UNSTOP_WHEN_UNFOCUS

///Debug end
#include "kernel.h"
//#include "mods.h"
#include "helping.h"
#include "@extraSFML/sfml-math.h"
#include "@extraSFML/sfml-layout.h"
#include "@easyXX/easyColor.h"
#include "@easyXX/easyEvents.h"
#include "@easyXX/easyFont.h"
#include "@easyXX/easyTex.h"
#include "TerrarianNew.h"
#include "macros.h"
#include "scdta.h"
#include "SFML/Audio.hpp"
//#include "@cppPy/CppPy.h"
#include "@shader/easyShader.hpp"
#include "@GameObjects/@Surroundings/surroundings.h"
#include "@Mod/dllLoadKernel.h"
#include "@Game/manager.h"
#include "@Game/Registry.h"
#include <psapi.h>
#include <process.h>
#include <stdlib.h>
#include "Translator.h"

//Smaller than 0 means no restrict
#define RESTRICT_FRAME_LIMIT -1
#define UPDATE_FPS_PER_SEC 2

#define timeGt "[" + getTime() + "]:"

#define MENU_MSC_RANGE_MIN 0
#define MENU_MSC_RANGE_MAX 0

//#define LOGO_TEXT_SCALE 1

//Game Controls
#define SCALING_PERCENT (float)(0.0003)
#define ROTATING_PERCENT (float)(0.03)
#define SUN_MOON_RT checkDebug(0.1,0.01)
#define SC_MPEC 0.03
#define RT_MPEC 6
#define MV_CLD checkDebug(1.0,0.1)
#define CLD_C 16
#define FLEX_SPEED 1.5
#define CLOUD_C 8
#define BACK_MOVE checkDebug(1.0,0.1)

#define SONG_SPLIT 4

#define ONLY_INIT_ONCE_INIT  static bool inited_oios = false
#define ONLY_INIT_ONCE_START if(!inited_oios){inited_oios = true;
#define ONLY_INIT_ONCE_END }

#define ONLY_INIT_ONCE_INIT_I(i)  static bool inited_oios##i = false
#define ONLY_INIT_ONCE_START_I(i) if(!inited_oios##i){inited_oios##i = true;
#define EASY_LOG(h,s,o) sl(mo,"\n-"+ h + "----Log Start---------------------\n" + s + "\n--------------Log End-----------------\n");

///FLEX_CHOICES
#define FLEX_SUN 1
#define FLEX_MOON 2

using namespace std;
using namespace sf;
using namespace rapidjson;

#ifdef MODE_DEBUG
#define FPS_Regular {static cck::Clock averTimer;static cck::Clock fpsTimer;static double aver = 0;static double fpsAll = 0;static double splitT = 0;static Text fpsT("fps:detecting\nmpf:detecting",*dfont,16);fpsT.setOutlineColor(Color::Black);fpsT.setOutlineThickness(1);double eq = (double)fpsTimer.GetOffset();\
splitT += eq;if(eq != 0 && splitT > FPerS(UPDATE_FPS_PER_SEC)){\
splitT = 0;double fps = (double)1000 / eq;fpsAll += fps;if(averTimer.Now().offset >= 1500){averTimer.GetOffset();if(aver == 0){aver = fpsAll / (4*UPDATE_FPS_PER_SEC);}else{aver = (aver + fpsAll / (1.5*UPDATE_FPS_PER_SEC))/2;}fpsAll = 0;}fpsT.setFillColor(Color::Yellow);\
fpsT.setString("fps:" + to_string((int)fps) + "\nmpf:" + to_string(eq) + "\nAver:" + to_string((int)aver));}window.draw(fpsT);}
#else
#define FPS_Regular {static cck::Clock fpsTimer;static double splitT = 0;static Text fpsT("fps:detecting",*dfont,16);fpsT.setOutlineColor(Color::Black);fpsT.setOutlineThickness(1);double eq = (double)fpsTimer.GetOffset();\
splitT += eq;if(eq != 0 && splitT > FPerS(UPDATE_FPS_PER_SEC)){\
splitT = 0;double fps = (double)1000 / eq;fpsT.setFillColor(Color::Yellow);\
fpsT.setString("fps:" + to_string((int)fps));}window.draw(fpsT);}
#endif // MODE_DEBUG

#ifdef ENABLE_FPS
#define showFpsDB FPS_Regular
#else
#define showFpsDB
#endif // ENABLE_FPS

struct GameSceneContacting{
private:
    bool m_boolInit;
    int m_boolSize;
public:
    vector<bool> bools;

    GameSceneContacting(int boolSize = 0,bool boolInit = false){
        m_boolInit = boolInit;
        m_boolSize = boolSize;
    }

    void clear(){
        bools.resize(m_boolSize,m_boolInit);
    }
};

///PlaneId配合Object
///0x[PLANEID][OBJECT_ID]
///栗子:
///0x00000001 //PlaneId 为0的id为1的对象

///Windows
int DrawStates(RenderWindow & window);
//PlaneID:0
int CartoonStartUp(RenderWindow & window);
//PlaneID:1
int loadingProc(RenderWindow & window);
//PlaneID:2
int mainMenu(RenderWindow & window,GameSceneContacting * gsc);
//PlaneID:3
int modsWindow(RenderWindow & window,GameSceneContacting * gsc,RenderTexture * rt = NULL);
//PlaneID:4
int modsExtraWindow(RenderWindow & window);
//PlaneID:5
int gameWindow(RenderWindow & window);
//PlaneID:6
int mainMenuBackground(RenderWindow & window,GameSceneContacting * gsc,RenderTexture * rt=NULL);
//PlaneID:7
int settingWindow(RenderWindow & window);

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

///游戏的整体设置在这里添加
struct GameGlobalConfig{
    string languageId;
};

void LoadGameGlobalConfig(string path,GameGlobalConfig& ggc);
void SaveGameGlobalConfig(string path,GameGlobalConfig& ggc);

/*
#define SEED_GEN_HASH_TIME 4

struct GameInfo{
    unsigned int seed;
    static unsigned int SimpleSeedGen(string seedIn){
        unsigned int rt_seed = 0;
        rt_seed += multiHash(singleHash(seedIn),SEED_GEN_HASH_TIME);
        rt_seed += 114514;
        return rt_seed;
    }
    static unsigned int singleHash(string seedIn){
        unsigned int rt_seed = 0;
        for(char singleChar : seedIn){
            rt_seed += (int)singleChar;
        }
        return rt_seed;
    }
    static unsigned int multiHash(unsigned int single,unsigned int exectime){
        unsigned int rt = 0;
        for(unsigned int i = 0;i < exectime;++i){
            rt += single * (i * 3 + 114514) - 114514;
        }
        return rt;
    }
};
*/
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

/*
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
*/

/*
string getPyExc(PyObject * v){
    PyObject * fmtExc = PyObject_Str(v);
    return PyObjectToString(fmtExc);
}
*/

//Lamda to one!!!
#define ml(x,m) ([&](void)->auto{auto v = x;m;return x;})()

string sav_sep = "\n";
string operator +(string a,vector<string> b){
    string ret = a;
    for(string & s : b){
        ret += s + sav_sep;
    }
    return ret;
}
string operator +(vector<string> b,string a){
    string ret = "";
    for(string & s : b){
        ret += s + sav_sep;
    }
    ret += a;
    return ret;
}

void OutputMods(ModsHelper & mh);

struct MemTp{float mem;float vmem;};
struct GlMem{float percent;float phy;float vir;float usephy;};

MemTp GetCurrentMemoryUsage();
GlMem GetGlobalMemoryUsage();

#ifdef BUILD_ON_WINDOWS
string _Windows_getCPUInfo(){
    long lRet;
	HKEY hKey;
	CHAR tchData[1024];
	DWORD dwSize;

	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("Hardware\\Description\\System\\CentralProcessor\\0"),0,KEY_QUERY_VALUE,&hKey);

	if(lRet == ERROR_SUCCESS){
		dwSize = 1024;
		lRet = RegQueryValueEx(hKey,_T("ProcessorNameString"),0,0,(LPBYTE)tchData,&dwSize);

		tchData[dwSize] = '\0';
		RegCloseKey(hKey);
		if(lRet == ERROR_SUCCESS){
			return string(tchData);
		}else{
			return "Unknown";
		}
	}
	return "Unknown";
}
#endif // BUILD_ON_WINDOWS


struct CPUInfo{
    string CpuID;
    CPUInfo(){
        #ifdef BUILD_ON_WINDOWS
        ///Windows平台下利用WMIC获取,但是会有弹窗QAQ
        this->CpuID = _Windows_getCPUInfo();
        #endif // BUILD_ON_WINDOWS
    }
};

string translateSeconds(int msecs){
    int sec = msecs%60;
    msecs /= 60;
    int min = msecs % 60;
    msecs /= 60;
    int hour = msecs % 60;
    msecs /= 60;
    int day = msecs % 24;
    msecs /= 24;
    int year = msecs % 356;
    string ret = "";
    if(year != 0)ret += to_string(year) + "y ";
    if(day != 0)ret += to_string(day) + "d ";
    if(hour != 0)ret += to_string(hour) + "h ";
    if(min != 0)ret += to_string(min) + "m ";
    if(sec != 0)ret += to_string(sec) + "s";
    return ret;
}

#endif // MAIN_HPP_INCLUDED
