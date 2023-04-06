/**
自己选择了SFML,OpenGL,就别放弃！！！！！！！！！
不要为了一点难事而放弃！！！！！
加油！~(>o<)~!!!!
**/
///
///大注意：全盘编译记得换上c++14,因为脑残c++17把byte搞得一团乱
//TODO:do not forget to change the debug io mode
#include "main.hpp"
#include "MusicController.h"

using namespace std;
using namespace sf;
using namespace cck;
using namespace rapidjson;
using namespace game;

//This is the id of the current scene
int sceneId = 1;//If debug,skip the first scene
cck::Clock timer = cck::Clock(false);
DWORD startTime;

//Lock
//PyLock pylock;

//Scene Information
Color clearSceneColor = Color::Black;

//Application Info
HWND windowHwnd = NULL;
Vector2i winSize = Vector2i(800,600);
bool focusing = true;
int tempLoadingValue = 0;
pthread_t worker1;//Worker Thread 1
char * appData;
bool hasEvent;
bool modLoadingGood = true;
vector<Event> events;
HelperEvents he;

//Game Info
Font * dfont;
TexturesHelper texs;
LogSaver ls;
GameManager gm;
Player player;
#include "dataStrs.h"
#include "@Game/TILES.h"
Register reg(gm);
Translator t;
GameGlobalConfig ggc;
MusicController museC;

//Mods
ModsHelper mh;

cck::Clock runningClock;
map<int,LayoutController> lc;

#define ALL_LOG 0
#define ONLY_IMPORTANT 1

#define LOG_LEVEL ALL_LOG

#define addLog(lg) ls << (string(timeGt)+ to_string(runningClock.GetALLTime())  + string("ms:") +string((lg))+"\n")

///Switch Log Storing Level
#if LOG_LEVEL == ALL_LOG
#define il addLog
#else
#define il
#endif // LOG_LEVEL

#define sl(o,lg) o += (string(timeGt)+ to_string(runningClock.GetALLTime())  + string("ms:") +string((lg))+"\n");
#define al addLog
#define sepl ls << string("-----------------------------------------------------\n")
#define ssep ls << string("*****************************************************\n")

#define ssil(x) ssep;il(x);ssep

ShaderStatus shaderStatus;
CPUInfo cpuInfo;
bool reFrameLimitWhenUnFocus = true;
int main(){
    srand(time(0));
    al("Starting this application and checking clocks...");
    {
        ///初始化nanosecond或者milli seconds
        cck::Clock clk000;
        if(cck::Clock::useHTimer){
            al("Using nanosecond timer:Windows QueryPerformanceCounter");
        }else{
            al("Using normal timer:Windows media timer");
        }
    }

    al("Loading Game Configs &Translations...");
    LoadGameGlobalConfig(GLOBAL_GAME_CONFIG_PATH,ggc);
    t.LoadTranslateFiles("./res/translations/");
    ///在这里尝试加载翻译
    al("Forming translations...");
    switch(t.LoadTranslate(ggc.languageId)){
    case 0:
        al("Translations are formed well!");
        break;
    case -1:
        al("Bad translations forming,we would use inner data(inner en_us) to deal texts.");
        break;
    case -2:
        al("Though translation is lost,we found default translation still available.");
        break;
    }

    al("Loading font...");
    if(MegaFont::loadDefault()){
        al("Error:default font missing...");
        EAssert(t.Translate("gm.err.fnt","Fail to load default font!Maybe you should re-install this application!").GetGBK().c_str());
        return -1;
    }
    al("Now are building fonts...");
    dfont = MegaFont::getDefaultFont();
    //Font Data
    //Mistake 0:不要用=复制进行初始化，要用如下初始化，析构函数会删掉加载的dfont
    //Mistake 1:一定要对default font 与normal font区别处理！否则会因为析构函数重复删去内存导致0xC0000005(内存问题)
    MegaFont defaultFontMega(dfont,true,true);
    al("Loading extra system fonts...");
    LoadFonts();

    al("Creating main windows...");
    RenderWindow window(sf::VideoMode(winSize.x,winSize.y),"UnlimitedLife Mod Loader",Style::Titlebar | Style::Close);
    windowHwnd = window.getSystemHandle();

    al("Restrict update frame limit...");
    if(RESTRICT_FRAME_LIMIT >= 0)window.setFramerateLimit(RESTRICT_FRAME_LIMIT);//Set frame limit to 120 fps

    al("Starting to check opengl...");
    ShaderInfo(shaderStatus);
    al("Here are the opengl result:" +
             _s("\nIs shader available:") + (shaderStatus.isAvailable?"yes":"no") +
             _s("\nOpenGL version     :") + shaderStatus.GLVersion +
             "\nVendor name        :" + shaderStatus.vendor +
             "\nRenderer token     :" + shaderStatus.rendererToken +
             "\nGlu Version        :" + shaderStatus.GLUVersion);
    al("Showing CPU Info...");
    al(string("CPU info:")+ "\n    CPU Id:" +cpuInfo.CpuID);

    ///INFORM:更新窗口大小需要更新gm的w,h
    gm.w = winSize.x;
    gm.h = winSize.y;

    al("The application started to dealing UI...");
    while (window.isOpen())
    {
        he.Origin();
        hasEvent = false;
        events.clear();
        //Processing events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed){
                window.close();
                ///Do Nothing
            }
            else if(event.type == sf::Event::LostFocus){
                focusing = false;
                #ifndef UNSTOP_WHEN_UNFOCUS
                timer.Pause();
                museC.PauseAll();
                #endif // UNSTOP_WHEN_UNFOCUS
                if(reFrameLimitWhenUnFocus){
                    window.setFramerateLimit(24);
                    //al("Frame limited to 24fps...");
                }
            }else if(event.type == sf::Event::Resized){
                winSize = {(int)event.size.width,(int)event.size.height};
            }else if(event.type == sf::Event::GainedFocus){
                focusing = true;
                #ifndef UNSTOP_WHEN_UNFOCUS
                timer.Resume();
                museC.ResumeAll();
                #endif // UNSTOP_WHEN_UNFOCUS
                if(RESTRICT_FRAME_LIMIT >= 0)window.setFramerateLimit(RESTRICT_FRAME_LIMIT);
                else window.setFramerateLimit(999999);
                //if(reFrameLimitWhenUnFocus)al("Unrestricted frame limit...");
            }
            else{
                ///Dealing other events
                if(event.type == sf::Event::MouseButtonReleased){
                    he.mouseRel = events.size();
                }else if(event.type == sf::Event::MouseButtonPressed){
                    he.mousePre = events.size();
                }else if(event.type == sf::Event::MouseMoved){
                    he.mouseMov = events.size();
                }else if(event.type == sf::Event::KeyPressed){
                    he.keyPre = events.size();
                }else if(event.type == sf::Event::KeyReleased){
                    he.keyRel = events.size();
                }
                ///Push events
                hasEvent = true;
                events.push_back(event);
            }
        }
        //Check Drawing Fail or Suc
        if(DrawStates(window) == EXECUTE_FAI)break;
    }
    al("Saving Configs");
    SaveGameGlobalConfig(GLOBAL_GAME_CONFIG_PATH,ggc);
    ///TODO::Save all the statues
    al("Game terminated...");
    return EXIT_SUCCESS;
}

int DrawStates(RenderWindow & window){
    static sf::RenderTexture rt;
    static bool inited = false;
    static bool suc = false;
    static cck::Clock rsc;
    static RenderTexture * rtb = NULL;
    int returnResult = EXECUTE_SUC;

    if(rsc.Now().offset >= 500){
        rsc.GetOffset();
        museC.Update();
    }

    //Drawing Events
    window.clear(clearSceneColor);
    switch(sceneId){
        case SC_LOADING:{
            returnResult = loadingProc(window);
            break;
        }
        case SC_MENU:{
            returnResult = mainMenuBackground(window,NULL);
            returnResult = returnResult | mainMenu(window,NULL);
            break;
        }
        case SC_MODS:{
            if(!inited){
                inited = true;
                suc = rt.create(winSize.x,winSize.y);
                if(suc)rtb = &rt;
            }
            returnResult = mainMenuBackground(window,NULL,rtb);
            returnResult = returnResult | modsWindow(window,NULL,rtb);
            break;
        }
        case SC_WORLD:{
            ///内嵌调用mainMenuBackground
            returnResult = gameWindow(window);
            break;
        }
        case SC_SETTING:{
            returnResult = mainMenuBackground(window,NULL);
            returnResult = returnResult | settingWindow(window);
            break;
        }
        default:
            break;
    }
    // No need: >_< ///Maybe TODO: move it to draw function,because drawable will be destroy
    //Display The Window
    window.display();
    return returnResult;
}

int settingWindow(RenderWindow & window){
    static unsigned int cdx = 0;
    static vector<string> lanAccesList;
    static unsigned int lanIndex = 0;
    ONLY_INIT_ONCE_INIT;
    clearSceneColor = Color(0,0,0);

    ONLY_INIT_ONCE_START
        ///构建语言列表,与定位
        bool inc = true;
        for(auto& x : t.summTrans){
            if(!x.first.compare(ggc.languageId))inc = false;
            if(inc)++lanIndex;
            lanAccesList.push_back(x.first);
        }

        ///MultiTranslte的一个应用
        Text * tx = new Text(MultiTranslate(t,"setting.choseLan","Language:%s",
                    MultiEnString::Utf8,
                    t.Translate(VERIFY_TOKEN,"Inner(en_us)").GetUTF8().c_str()).GetUTF16(),*dfont,24);
        tx->setOutlineThickness(1);
        tx->setOutlineColor(Color(0,0,0));
        lc.insert(make_pair(0,LayoutController()));
        lc[0].Append(tx,[](Text * txt,void * d){
            SocSettingWin & wic = *((SocSettingWin*)d);
            Translator * tt = (Translator*)wic.translator;
            if((wic.ac)->size() == 0)return -1;
            *wic.li += 1;
            if(*(wic.li) >= (wic.ac)->size())*(wic.li) = 0;
            tt->LoadTranslate((*(wic.ac))[*(wic.li)]);
            ///cout << *(wic.li) << endl;
            txt->setString(MultiTranslate((*tt),"setting.choseLan","Language:%s",
                    MultiEnString::Utf8,
                    ((*tt)).Translate(VERIFY_TOKEN,"Inner(en_us)").GetUTF8().c_str()).GetUTF16());
            ggc.languageId = tt->Translate(ACCESS_TOKEN,"Inner(en_us)").GetUTF8();
            lc[0].Set(0,0)->SetTextsAlign(ORI_CENTER)->SetTextPadding(8)->StaticForm(0,100,winSize.x,winSize.y);
            return 0;
        });
        lc[0].Set(0,0)->SetTextsAlign(ORI_CENTER)->SetTextPadding(8)->StaticForm(0,100,winSize.x,winSize.y);
    ONLY_INIT_ONCE_END

    if(he.mouseMov != -1){
        lc[cdx].PositionDetects(Vector2f(events[he.mouseMov].mouseMove.x,events[he.mouseMov].mouseMove.y),
        [&](Text * tx,bool target){
            if(target){
                tx->setFillColor(ColorMoreXX(white,0.8));
            }else{
                tx->setFillColor(white);
            }
        });
    }else if(he.mouseRel != -1){
        Event& e = events[he.mouseRel];
        if(e.mouseButton.button == Mouse::Left){
            ///当没有碰到时返回UNREACHABLE(1145141919),因此要做好index越界处理,已在RunOnClick中处理完毕
            unsigned int id = lc[cdx].PositionDetects(Vector2f(e.mouseButton.x,e.mouseButton.y));
            //cout << id << endl;
            SocSettingWin tus = {&lanAccesList,&lanIndex,&t};
            lc[cdx].RunOnClick(id,&tus);
        }
    }else if(he.keyPre != -1){
        Event e = events[he.keyPre];
        if(e.key.code == Keyboard::Escape){
            sceneId = SC_MENU;
        }
    }
    lc[cdx].draw(window);
    showFpsDB;
    return EXECUTE_SUC;
}

int gameWindow(RenderWindow & window){
    static Texture tex;
    static AbstractTile ab(0);
    static Vector2i od = {-999999,-999999};
    static Pt2Di odpos = {-1145141919,-1145141919};
    //F3调试 未来加入data.debug字段时存储
    static bool debugMode = false;
    static cck::Clock movement;
    ///Initializing initializing vars
    ONLY_INIT_ONCE_INIT;
    initEPI;
    ///End
    ///小贴士：在场景发生变化时InitEPI要重置：调用ReInitEPI
    enterPerInit
        gm.ResumeGame();
        gm.LoadPerm();
        od = {-999999,-999999};
        odpos = {-1145141919,-1145141919};
    endEPI
    clearSceneColor = Color(0,0,0);

    ONLY_INIT_ONCE_START
        Sprite playerSp;
        gm.StartWorkerThread(1);
        ///This statement is used to initialize time summary
        gm.ResumeGame();//这一句话用于初始化时间统计
        clearSceneColor = Color::Black;//Use black to fill the backgroud
        player.setBasicInformation(114514).setCollisionPercen(0.5,1).setPosition(0,0,0).setDyChunkInfo(1);
        gm.Bind(player,window.getDefaultView());
        if(!tex.loadFromFile(PLAYER_BASE "pl_test.png"))exit(-1145142);
        playerSp.setOrigin(tex.getSize().x/2, tex.getSize().y);
        playerSp.setPosition(gm.w/2,gm.h/2);
        playerSp.setTexture(tex);
        player.texSz = toFloat(tex.getSize());
        playerSp.setScale(player.GetFormedScale(AbstractTile::len));
        gm.SetPlayerSprite(playerSp);
        gm.BindMusicController(&museC);
    ONLY_INIT_ONCE_END

    if(ChunkId(player.position) != od){
        //Chunk Id Updates
        gm.UpdateDySingle();
        od = ChunkId(player.position);
    }
    if(toInt(player.position) != odpos){
        //Position Updates
        odpos = toInt(player.position);
        gm.UpdateView();
    }

    //window.draw(CH::buildSprite(gm.tileTexs,&ab,{0,0}));
    gm.Paint(window);

    if(movement.checkEslapseReset(10) && focusing){
        bool cancleMove = false;
        player.veclocity = {0,0};
        //Base movement events
        if(GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP)){
            player.veclocity += {0,-1};
        }
        if(GetAsyncKeyState('S') || GetAsyncKeyState(VK_DOWN)){
            player.veclocity += {0,1};
        }
        if(GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT)){
            player.veclocity += {-1,0};
        }
        if(GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT)){
            player.veclocity += {1,0};
        }
        if(player.veclocity.x != 0 || player.veclocity.y != 0){
            player.veclocity = Normalize(player.veclocity) / 16.0;
            loopv(xdx,3){
                loopv(ydx,3){
                    AbstractTile * b = gm.vg(floorPt(player.position) - Pt2Di(xdx - 1,ydx - 1),1);
                    if(b && !b->deprecated_v){
                        if(b->GenCollider().intersects(player.GetRect())){
                            cancleMove = true;
                        }
                    }
                    if(cancleMove)break;
                }
                if(cancleMove)break;
            }
            if(!cancleMove)player.Move(player.veclocity.x,player.veclocity.y);
        }
    }

    if(he.keyPre != -1){
        ExtractEvent(keyPre);
        if(MatchEKey(Keyboard::B) && e.key.control){
            gm.showColliders = !gm.showColliders;
        }else if(MatchEKey(Keyboard::T) && e.key.control){
            gm.tblkSp.clear();
            gm.templateSprites.clear();
            gm.blkTexs.clear();
            gm.tileTexs.clear();
            reg.RegisterTiles(tiles);
            reg.RegisterBlocks(blocks);
        }else if(MatchEKey(Keyboard::F3)){
            debugMode = !debugMode;
        }else if(MatchEKey(Keyboard::Escape)){
            sceneId = SC_MENU;
            ReInitEPI;
            gm.EndupGame();
        }
        gm.OnPress(e);
    }
    showFpsDB
    if(debugMode){
        MemTp mems = GetCurrentMemoryUsage();
        GlMem gmem = GetGlobalMemoryUsage();
        sf::Text text(
            string("--------------------------------------------")
                + "\nUnlimited Life F3 Debug v0.0:\n"
                + "\nGlobal"
                + "\n   Real Time: " + timeGt
                + "\n   Global Memory Usage: " + to_string((int)gmem.usephy) + "/" + to_string((int)gmem.phy) + "M  " + to_string((int)(gmem.percent*100)) + "%"
                + "\n   CPU:" + cpuInfo.CpuID
                + "\n   Render Token(GPU): " + shaderStatus.rendererToken
                + "\n   OpenGL: " + shaderStatus.GLVersion.substr(0,3)
                + "\n   GLU Version: " + shaderStatus.GLUVersion
                + "\n   GLSL Version: " + shaderStatus.GLSLVersion
                + "\n   Shader Stat: " + (shaderStatus.isAvailable?"Available":"Unavailable")
                + "\nWorld"
                + "\n   Played Time:" + translateSeconds(gm.playedTime.GetALLTime()/1000)
                + "\nPlayer"
                + "\n   Position: " + VSTR_MAKE(player.position)
                + "\n   Current Chunk Id: " + VSTR_MAKE(ChunkId(player.position))
                + "\n   Dimension Id: " + to_string(player.dimension)
                + "\n   UUID Local: " + to_string(player.uuid_local)
                + "\nCurrent Application"
                + "\n   Memory Usage: " + to_string((int)mems.mem) + "M"
                + "\n   VMem Usage: " + to_string((int)mems.vmem) + "M"
                + "\n"
        ,*dfont,12);
        text.setPosition(0,60);
        text.setFillColor(Color::White);
        sf::RectangleShape fil;
        fil.setPosition(text.getGlobalBounds().left,text.getGlobalBounds().top);
        fil.setSize({text.getGlobalBounds().width,text.getGlobalBounds().height});
        fil.setFillColor(Color(88,88,88,128));
        window.draw(fil);
        window.draw(text);
    }
    return EXECUTE_SUC;
}

int mainMenuBackground(RenderWindow & window,GameSceneContacting * gsc,RenderTexture * rt){
    static float rTheta = 0;
    static float dec;
    static RenderTexture mb;
    static vector<Texture*> tx = {texs["bg"]};
    static cck::Clock delta;

    float deltaTime = delta.GetOffset();
    float deltaMove = deltaTime / 18;

    ONLY_INIT_ONCE_INIT;
    ONLY_INIT_ONCE_START
        if(!mb.create(window.getSize().x,window.getSize().y)){
            al("Cannot create buffer!At main menu!");
            EAssertExEx(window.getSystemHandle(),"Unable to create a buffer to draw images!","Error");
            exit(-1);
        }
        /**
        issue:不知为何,游戏第一帧的deltaTime特别大，因此，须在此处再次赋值
        */
        rTheta = 140;
    ONLY_INIT_ONCE_END

    rTheta += SUN_MOON_RT * deltaMove;

    if(rTheta >= 360){
        rTheta = 0;
    }

    dec = sin(deg2rad(rTheta / 2));

    if(tx[0]){
        Sprite sp(*(tx[0]));
        float sc = (float)window.getSize().x / sp.getTexture()->getSize().x;
        sp.setScale(sc,sc);
        mb.draw(sp);
    }

    block(Repaint){
        Sprite sp(mb.getTexture());
        sp.scale(1,-1);
        sp.setColor(ColorMoreXX(Color(255,255,255),(dec + 0.1) > 1 ? 1 : dec + 0.1));
        sp.setPosition(0,mb.getSize().y);
        if(shaderStatus.isAvailable){
            window.draw(sp);
            if(rt)rt->draw(sp);
        }else{
            window.draw(sp);
            if(rt)rt->draw(sp);
        }
    }
    return EXECUTE_SUC;
}

int mainMenu(RenderWindow & window,GameSceneContacting * gsc){
    static bool musing = false;
    Vector2f pos(Mouse::getPosition(window).x,Mouse::getPosition(window).y);

    Text logoSp(t.Translate("game.defaultCaption","UnlimitedLife").GetUTF16(),*dfont,48);
    logoSp.setOutlineThickness(1);
    logoSp.setOutlineColor(Color::Black);

    FloatRect fr = logoSp.getGlobalBounds();
    logoSp.setPosition(setPosRelative(fr,winSize,PosCenter,PosPercent,0,0.15));

    window.draw(logoSp);

    if(!musing && rand()%100000 > 99500){
        musing = true;
        museC.AppendPlay(musics[0],false,"menuMuse");
    }

    if(musing && !museC["menuMuse"]){
        musing = false;
    }

    ///Drawing Main Menu
    block(Drawing Main Menu){
        static Color pmc = white;
        Text StartGame(t.Translate("game.menu.startGame","Start Game").GetUTF16(),*dfont,28);
        Text Mods(t.Translate("game.menu.modList","Mod List").GetUTF16(),*dfont,28);
        Text Settings(t.Translate("game.menu.settings","Settings").GetUTF16(),*dfont,28);
        Text QuitGame(t.Translate("game.menu.exit","Exit").GetUTF16(),*dfont,28);

        StartGame.setOutlineThickness(1);
        StartGame.setOutlineColor(Color::Black);
        Mods.setOutlineThickness(1);
        Mods.setOutlineColor(Color::Black);
        QuitGame.setOutlineThickness(1);
        QuitGame.setOutlineColor(Color::Black);
        Settings.setOutlineThickness(1);
        Settings.setOutlineColor(Color::Black);

        StartGame.setPosition(setPosRelative(StartGame.getLocalBounds(),winSize,PosPercent,PosPercent,0.1,0.5));
        Mods.setPosition(setPosRelative(Mods.getLocalBounds(),winSize,PosPercent,PosPercent,0.1,0.6));
        Settings.setPosition(setPosRelative(Settings.getLocalBounds(),winSize,PosPercent,PosPercent,0.1,0.7));
        QuitGame.setPosition(setPosRelative(QuitGame.getLocalBounds(),winSize,PosPercent,PosPercent,0.1,0.8));

        if(he.mouseRel != -1){
            if(QuitGame.getGlobalBounds().contains(pos)){
                ///Store Logs When The Application Quits
                ls.close();
                window.close();
            }else if(StartGame.getGlobalBounds().contains(pos)){
                sceneId = SC_WORLD;
            }else if(Settings.getGlobalBounds().contains(pos)){
                sceneId = SC_SETTING;
            }else if(Mods.getGlobalBounds().contains(pos)){
                sceneId = SC_MODS;
            }
        }

        if(QuitGame.getGlobalBounds().contains(pos)){
            init4of2(QuitGame,StartGame,Mods,Settings,setScale,1.1,1.1,1,1);
            init41of2(QuitGame,StartGame,Mods,Settings,setFillColor,ColorMoreXX(pmc,0.8),pmc);
        }else if(StartGame.getGlobalBounds().contains(pos)){
            init4of2(StartGame,QuitGame,Mods,Settings,setScale,1.1,1.1,1,1);
            init41of2(StartGame,QuitGame,Mods,Settings,setFillColor,ColorMoreXX(pmc,0.8),pmc);
        }else if(Mods.getGlobalBounds().contains(pos)){
            init4of2(Mods,QuitGame,StartGame,Settings,setScale,1.1,1.1,1,1);
            init41of2(Mods,QuitGame,StartGame,Settings,setFillColor,ColorMoreXX(pmc,0.8),pmc);
        }else if(Settings.getGlobalBounds().contains(pos)){
            init4of2(Settings,QuitGame,StartGame,Mods,setScale,1.1,1.1,1,1);
            init41of2(Settings,QuitGame,StartGame,Mods,setFillColor,ColorMoreXX(pmc,0.8),pmc);
        }

        window.draw(StartGame);
        window.draw(Mods);
        window.draw(Settings);
        window.draw(QuitGame);
    }

    ///This macro dosen't need to have a ; as the end
    showFpsDB

    return EXECUTE_SUC;
}

int loadingProc(RenderWindow & window){
    static bool startedWork = false;
    static LoadingProgress loadProg;
    static bool invoked = false;
    /*@Steps
        1.Loading Game Main Textures
        2.Preparing for Next Scene
        3.Loading recipes & game items' data
        4.loading for mods
        5.clean up memory
    */
    clearSceneColor = Color::Black;


    ///Drawing Stuffs///
    Text logoSp("UnlimitedLife",*dfont,48);
    FloatRect fr = logoSp.getGlobalBounds();
    logoSp.setPosition(setPosRelative(fr,winSize,PosCenter,PosPercent,0,0.15));
    window.draw(logoSp);
    ///Drawing Icon End///

    ///Start Tasks///
    if(!startedWork){
        al("Starting loading scene...");
        startedWork = true;
        al("Creating threads to dealing loading...");
        ///Start Working
        pthread_create(&worker1,NULL,loadingState,&loadProg);
        pthread_detach(worker1);
        al("Threads are running...Thread id:" + to_string(worker1));
    }

    ///Drawing Processing Value///
    //Checking Staues
    Text nowWhat(loadProg.nowLoading,*dfont,24);
    Text nowProg("Now progress:"+to_string(loadProg.nowProg) + "/100",*dfont,16);
    Text allProg("All  progress:"+to_string(loadProg.allProg) + "/100",*dfont,16);

    //Use Text set
    nowWhat.setPosition(0,0);
    nowWhat.setOutlineColor(Color::White);
    nowWhat.setFillColor(Color::White);
    nowWhat.setPosition(setPosRelative(nowWhat.getLocalBounds(),winSize,PosCenter,PosPercent,0,0.6));

    //Use Text set
    nowProg.setPosition(0,0);
    nowProg.setOutlineColor(Color::White);
    nowProg.setFillColor(Color::White);
    nowProg.setPosition(setPosRelative(allProg.getLocalBounds(),winSize,PosCenter,PosPercent,0,0.65));

    //Use Text set
    allProg.setPosition(0,0);
    allProg.setOutlineColor(Color::White);
    allProg.setFillColor(Color::White);
    allProg.setPosition(setPosRelative(allProg.getLocalBounds(),winSize,PosCenter,PosPercent,0,0.68));

    window.draw(nowWhat);
    window.draw(nowProg);
    window.draw(allProg);

    ///End Up Drawing///

    if(loadProg.invokedError){
        al("Invoked an error:" + loadProg.singalError);
        invoked = true;
        loadProg.invokedError = false;
    }

    ///End Up///
    ///TODO:delete 1000 if need
    if(loadProg.allProg > 101){
        al("Load finished,checking all errors...");
        if(showingStrings.size() != 0){
            SetWindowText(window.getSystemHandle(),("UnlimitedLife:" + showingStrings[rand() % showingStrings.size()]).c_str());
        }else{
            window.setTitle("无尽人生");
        }
        ++sceneId;
        if(invoked){
            al("Making logs to store error in...");
            //时间+随机数混淆，尽量保证错误文件可以写入
            string filePath = string(appData) + "\\StudyAll\\MineTerraria\\Cache\\errorInvoke" + to_string(time(0)) + to_string(rand()%100) + ".log";
            ofstream writer;
            writer.open(filePath);
            if(!writer.is_open()){
                ///Big Issue;Do not tell the user!
                al("Open error storing file fail!We can't store errors!");
                EAssertEx(windowHwnd,t.Translate("gm.err.norm","There is something wrong in the mod loading process!").GetGBK().c_str());
            }else{
                al("Storing errors...");
                writer.write(loadProg.finalError.c_str(),loadProg.finalError.length());
                writer.close();
                string out = MultiTranslate(t,"gm.err.path","There is something wrong in the mod loading process!Errors were stored in %s",MultiEnString::UTF8,filePath.c_str()).GetGBK();
                EAssertEx(windowHwnd,out.c_str());
            }
            if(loadProg.isCritical){
                al("Some of the errors are critical!Telling the user about this...");
                EAssertEx(windowHwnd,t.Translate("gm.err.critical","This is a critical wrong!We suggest you to look at the log file!Or the game may crash easily!").GetGBK().c_str());
            }
        }
    }
    return EXECUTE_SUC;
}

int modsWindow(RenderWindow & window,[[maybe_unused]] GameSceneContacting * gsc,RenderTexture * rt){
    //static vector<ModShow> mods;
    static Shader glass;
    static bool usingShader = false;
    static VertexArray quad(Quads,4);
    ONLY_INIT_ONCE_INIT;
    initEPI;
    Vector2f pos(Mouse::getPosition(window).x,Mouse::getPosition(window).y);
    ONLY_INIT_ONCE_START
        if(shaderStatus.isAvailable){
            if(!glass.loadFromFile(shaders[SHADER_GLASS_LIKE_F],Shader::Fragment)){
                al("ErrorInvoked:Cannot load shader " + shaders[SHADER_GLASS_LIKE_F]);
            }else if(rt){
                usingShader = true;
                glass.setUniform(string("tex"),rt->getTexture());
            }
        }
        OutputMods(mh);
    ONLY_INIT_ONCE_END

    enterPerInit
        Vector2u winsz = window.getSize();
        quad[0].position = Vector2f(getPercentage(0.05,0,winsz.x),getPercentage(0.05,0,winsz.y));
        quad[1].position = Vector2f(getPercentage(0.95,0,winsz.x),getPercentage(0.05,0,winsz.y));
        quad[2].position = Vector2f(getPercentage(0.95,0,winsz.x),getPercentage(0.95,0,winsz.y));
        quad[3].position = Vector2f(getPercentage(0.05,0,winsz.x),getPercentage(0.95,0,winsz.y));
        quad[0].color = quad[1].color = quad[2].color = quad[3].color = Color(200,200,200,128);
        if(usingShader){
            Vector3f poses = Vector3f(quad[0].position.x,quad[0].position.y,quad[2].position.x);
            Vector3f target = Vector3f(0.f,0.f,_f(winSize.x));
            Vector2f extra = Vector2f(quad[2].position.y,_f(winSize.y));
            glass.setUniform("poses",poses);
            glass.setUniform("target",target);
            glass.setUniform("extra",extra);
        }
    endEPI

    ///Check Shader to draw
    if(usingShader)window.draw(quad,&glass);
    else {
        window.draw(quad);
    }
    /*for(ModShow & msc : mods){
        window.draw(msc.modName);
    }*/
    Text back2MainMenu(t.Translate("game.text.back","Back").GetUTF16(),*dfont,24);
    back2MainMenu.setFillColor(Color::White);
    back2MainMenu.setPosition(setPosRelative(back2MainMenu.getLocalBounds(),winSize,PosCenter,PosPercent,0,0.95));

    if(he.mouseRel != -1){
        if(back2MainMenu.getGlobalBounds().contains(pos)){
            sceneId = SC_MENU;
            ReInitEPI;
        }
    }else if(he.keyPre != -1){
        Event e = events[he.keyPre];
        if(e.key.code == Keyboard::Escape){
            sceneId = SC_MENU;
        }
    }
    window.draw(back2MainMenu);

    showFpsDB
    //着色器渲染

    return EXECUTE_SUC;
}

int modsExtraWindow(RenderWindow & window){
    return EXECUTE_SUC;
}

void tryCreateAppData(){

    al("Getting environment data...");
    appData = getenv("APPDATA");
    if(!appData){
        al("Getting the environment data fail!!!Exit this program...");
        EAssertEx(windowHwnd,"AppData Means Nothing!");
        exit(EXECUTE_FAI);
    }

    al("Check folders to store data in...");
    //Create Folders
    string folderChecking = "";
    for(const string & path : folders){
        folderChecking =  string(appData) + path;
        if(_access(folderChecking.c_str(),0)){//0表示只是检测是否存在
            al("Folder Not Exists!Creating " + folderChecking);
            mkdir(folderChecking.c_str());
        }
    }
}

void LoadFonts(){
    //Load Extra Fonts
    string fontPathList[] = {
    };
}

void * loadingState(void * storeIn){
    LoadingProgress * lp = (LoadingProgress *)storeIn;
    sepl;
    al("Initializing Loading Proc...");
    setStr("Initializing Loading Proc");
    //Get App Data
    al("Trying to check appdata exists...");
    tryCreateAppData();
    ///Init logs
    al("Initializing logger...");
    ls.initStoring(string(appData) + folders[FOLDER_CACHE] +string("\\log")+to_string(time(0))+string(".log"));
    if(ls.getStatus() == false){
        ///We cannot store logs.
        ls.closeStoring();
    }
    ///End
    al("Creating variables...");
    vector<string> paths;
    BasicInfo basicInfo;
    basicInfo.Language = basicInfo.English;
    basicInfo.dllKernelVersion = MOD_VER;
    unsigned int decT = 0;
    string readPath = appData;
    readPath += folders[2];

    ///Deleting cache...
    al("Deleting last logs....");
    {
        string cachePath = appData;
        cachePath += folders[FOLDER_CACHE];
        getFileNames(cachePath,paths);
        setStr("Deleting cache...");
        for(string & spth : paths){
            il("Deleted file:" + spth);
            DeleteFile(spth.c_str());
        }
        paths.clear();
    }

    string mo;//ModOutput

    al("Starting Loading Mods...");
    mo +="\n--------------------------";
    mo += "\nMod Engine Info:\n";
    mo += " Current Mod Engine:DynamicLinkLibrary Based Mod Loading Engine\n";
    mo +="  Engine API Version:" + to_string(MOD_VER) + "\n";
    mo += "------------------------\n\n";
    sl(mo,"GetMods List...");
    getFileNames(readPath,paths);
    ///Erase useless mods
    while(decT < paths.size()){
        //Has Differrent
        if(paths[decT].substr(paths[decT].find_last_of('.') + 1).compare("dll")){
            sl(mo,paths[decT] + " do not fits the suffix \".dll\"");
            paths.erase(paths.begin() + decT);
            continue;
        }
        ++decT;
    }
    block(Outputting Mods){
        string output = "\nMods are:\n";
        for(string & s : paths){
            output += s + "\n";
        }
        sl(mo,output);
    }
    sl(mo,"Starting Get Mods...");
    ModsHelper mht;
    ModsHelper mhtSwap;
    mht.mods = GetAllAvaliableMods(paths);
    //Checking Important Functions & Initializing
    for(Mod mod : mht.mods){
        _GetModInfo GMI = (_GetModInfo) GetProcAddress(mod.module,GET_MOD_DATA_FUNC);
        if(!GMI){
            sl(mo,"Detected file " + mod.path + " is not a mod!:DETAIL:NO GetModInfo func");
            continue;
        }else {
            mod.GMI = GMI;
        }
        _InitMod IM = (_InitMod) GetProcAddress(mod.module,INIT_MOD_FUNC);
        if(!IM){
            sl(mo,"Detected file " + mod.path + " is not a mod!:DETAIL:NO InitializeMod func");
            continue;
        }else {
            mod.IM = IM;
            mhtSwap.mods.push_back(mod);
        }
    }
    mht.mods.clear();
    //Initializing
    for(Mod mod : mhtSwap.mods){
        sl(mo,"Mod " + mod.path);
        sl(mo,"-----------------------------------------------------");
        setStr("Initializing ..." + mod.path.substr(mod.path.size()-10));
        mod.info = mod.GMI(basicInfo);
        try{
            if(!mod.info.thisObjIsValid){
                sl(mo,"Mod " + mod.path + " returns a bad value when get the info!");
            }else{
                mo += "Log:";
                mo += "\n" + mod.info.log;
                sl(mo,"-----------------------------------------------------\n");
                sl(mo,"Mod " + mod.info.name + " Path:" + mod.path + ":\nAuthor:" + mod.info.author + "\nDescription:" + mod.info.Description + "\n");
                ///Version Detect
                if(mod.info.dllKernelVersion != MOD_VER){
                    sl(mo,"Mod " + mod.info.name + " has a different MOD_API VERSION!CURRENT:" + to_string(MOD_VER));
                }else if(mod.info.failed){
                    sl(mo,"Mod" + mod.info.name + " failed when loading!!!!See the Mod's Log for more detail...");
                }else{
                    //*mod.info.test.iv = 1;
                    //*(string *)(mod.info.test.vv) = "Hello";
                    //SafeCall(mod.info.test.fn);
                    mht.mods.push_back(mod);
                }
            }
        }catch(...){
            sl(mo,"Oops,catch finally get an exception!");
        }
    }
    sl(mo,"Detecting UUIDS...");
    ///Discard Same Mods
    mhtSwap.mods.clear();
    {
        vector<string> uuids;
        for(Mod & m : mht.mods){
            bool sameUUID = false;
            for(string uuid : uuids){
                if(uuid.compare(m.info.packageUUID)){
                    sl(mo,"Detected Same UUID " + uuid + " at " + m.path);
                    sameUUID = true;
                }
            }
            if(!sameUUID){
                uuids.push_back(m.info.packageUUID);
                mhtSwap.mods.push_back(m);
            }
        }
    }
    sl(mo,"Finished...");
    {
        sl(mo,"Initializing Mods...");
        for(Mod & m : mhtSwap.mods){
            m.initInfo = m.IM(InitInfoOut(&showingStrings));
            if(m.initInfo.hasLog)EASY_LOG(m.info.name,m.initInfo.log,mo);
        }
    }
    al(mo);

    ///Last Step Value
    mh.mods = mhtSwap.mods;
    al("Loading system textures...");
    setStr("Loading System Textures...");
    Texture * stTex = NULL;
    for(unsigned int rd = 0;rd < texturePaths.size();rd++){
        stTex = new Texture();
        if(!stTex->loadFromFile(texturePaths[rd])){
            al("Can't load texture \"" + texturePaths[rd] + "\"This is a critical wrong!!!");
            lp->isCritical = true;
            invokeOne(timeGt + "[ERROR_CRITICAL_WRONG]:Fail to load texture:" + texturePaths[rd] +"!!!!!This may broke this game!!!!Maybe re-install this application can fix this problem!");
            texs.add(nameTexs[rd],NULL);
            delete stTex;
        }else{
            al("Texture loaded \"" + texturePaths[rd] + "\"");
            texs.add(nameTexs[rd],stTex);
        }
    }
    al("Loading Assets...");
    reg.RegisterTiles(tiles);
    reg.RegisterBlocks(blocks);

    al("Loading finished....Now will go to next scene...");
    ls.flush();
    setStr("Loading Finished!");
    Sleep(checkDebug(0,1000));
    setAProg(105);
    return NULL;
}

void OutputMods(ModsHelper & mh){
    sepl;
    al("Mod's Name UUID Author Description Version HMODULE");
    string ot = "";
    for(Mod & m : mh.mods){
        ot = m.info.name + " " + m.info.packageUUID + " " + m.info.author + " " + m.info.Description + " " + m.info.version.toString() + " " + to_string((unsigned long)m.module) + "\n";
    }
    al(ot);
    sepl;
}

void LoadGameGlobalConfig(string path,GameGlobalConfig& ggc){
    if(fileIO::check_exists((char*)path.c_str())){
        ifstream ifs(path);
        string dta = "";
        ifs >> ggc.languageId;
        ifs.close();
    }else{
        ggc.languageId = "en_us";
    }
}

void SaveGameGlobalConfig(string path,GameGlobalConfig& ggc){
    ofstream ofs(path);
    ofs << ggc.languageId << " ";
}
