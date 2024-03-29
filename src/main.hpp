#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED
///macros & tool functions
#include "helping.h"
///sfml extras
#include "@extraSFML/sfml-math.h"
#include "@extraSFML/sfml-layout.h"
///Easy conytroller
#include "@easyXX/easyColor.h"
#include "@easyXX/easyEvents.h"
#include "@easyXX/easyFont.h"
#include "@easyXX/easyTex.h"
///Terrain
#include "TerrarianNew.h"
///Simple macros
#include "macros.h"
///Scene data
#include "scdta.h"
///SFML Classes
#include "SFML/Audio.hpp"
///About Shader
#include "@shader/easyShader.hpp"
///Mods
#include "@Mod/dllLoadKernel.h"
#include "@Mod/Sockets.h"
///Game manager & register & translator
#include "@Game/manager.h"
#include "@Game/Registry.h"
#include "Translator.h"
///Reduce complile time
#include "expanded.h"

using namespace std;
using namespace sf;

///游戏的整体设置在这里添加
struct GameGlobalConfig{
    string languageId;
};

///Windows
int DrawStates(RenderWindow & window);
int loadingProc(RenderWindow & window);
int mainMenu(RenderWindow & window,GameSceneContacting * gsc);
int modsWindow(RenderWindow & window,GameSceneContacting * gsc,RenderTexture * rt = NULL);
int gameWindow(RenderWindow & window);
int mainMenuBackground(RenderWindow & window,GameSceneContacting * gsc,RenderTexture * rt=NULL);
int settingWindow(RenderWindow & window);

///States
void LoadFonts();
void * loadingState(void * storeIn);
void tryCreateAppData();

void LoadGameGlobalConfig(string path,GameGlobalConfig& ggc);
void SaveGameGlobalConfig(string path,GameGlobalConfig& ggc);
void OutputMods(ModsHelper & mh);
#endif // MAIN_HPP_INCLUDED
