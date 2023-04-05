/**
自己选择了SFML,OpenGL,就别放弃！！！！！！！！！
不要为了一点难事而放弃！！！！！
加油！~(>o<)~!!!!
**/
//TODO:do not forget to change the debug io mode
#include "main.hpp"
#include "MusicController.h"

using namespace std;
using namespace sf;
using namespace cck;
using namespace rapidjson;
using namespace game;

//This is the id of the current scene
int sceneId = checkDebug(1,0);//If debug,skip the first scene
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
    #ifdef NO_AUDIO
        bgm.setVolume(0);
    #endif // NO_AUDIO
    srand(time(0));
    al("Starting this application...");
    {
        al("Checking clocks...");
        if(cck::Clock::useHTimer){
            al("Using nanosecond timer:Windows QueryPerformanceCounter");
        }else{
            al("Using normal timer:Windows media timer");
        }
    }
    al("Loading Game Configs &Translations...");
    LoadGameGlobalConfig(GLOBAL_GAME_CONFIG_PATH,ggc);
    t.LoadTranslateFiles("./res/translations/");
    sepl;
    al("Loading font...");
    if(MegaFont::loadDefault()){
        al("Error:default font missing...");
        EAssert("Fail to load default font!Maybe you should re-install this application!");
        return -1;
    }
    /*
    al("Initializing mod executer,Python...");
    if(pylock.init() != EXECUTE_SUC){
        al("Error:Python initializes fail!");
        EAssert("Python inited fail!!!!!This application now cannot load mods!!!");
        modLoadingGood = false;
    }
    al("Initializes Python successfully!Now are building fonts...");
    */
    al("Now are building fonts...");
    dfont = MegaFont::getDefaultFont();
    //Font Data
    //Mistake 0:不要用=复制进行初始化，要用如下初始化，析构函数会删掉加载的dfont
    //Mistake 1:一定要对default font 与normal font区别处理！否则会因为析构函数重复删去内存导致0xC0000005(内存问题)
    MegaFont defaultFontMega(dfont,true,true);
    al("Loading extra system fonts...");
    LoadFonts();
    al("Creating main windows...");
    //Create Main Window
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
    al(string("CPU info:")
       + "\n    CPU Id:" +cpuInfo.CpuID
       );
    sepl;

    ///INFORM:更新窗口大小需要更新gm的w,h
    gm.w = winSize.x;
    gm.h = winSize.y;
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

int CartoonStartUp(RenderWindow & window){
    //Use static var to initialize
    //Static statement only initializes once
    static bool inited = false;
    //@deprecated
    //static const int alpha = 255;

    clearSceneColor = Color::Black;
    if(!inited){
        al("Initializing the first scene...");
        inited = true;
        timer.Start();
        startTime = timer.Now().all;
    }

    DWORD now = timer.Now().all;
    DWORD offset = now - startTime;
    //Step 1:The StudyAll Text Logo
    if(crangeEq(offset,0,800)){

        Text logoText("Made by StudyAll Studio",*dfont,64);
        Text useText("Made With C++ SFML Engine",*dfont,24);

        //Logo Text set
        logoText.setPosition(0,0);
        logoText.setOutlineColor(Color::White);
        logoText.setFillColor(Color::White);
        logoText.setPosition(setPosRelative(logoText.getLocalBounds(),winSize,PosCenter,PosPercent,0,0.2));
        //@deprecated
        //logoText.setFillColor(SetAlpha(logoText.getFillColor(),percentage(now,0,1600) * alpha));

        //Use Text set
        useText.setPosition(0,0);
        useText.setOutlineColor(Color::Yellow);
        useText.setFillColor(Color::Yellow);
        useText.setPosition(setPosRelative(useText.getLocalBounds(),winSize,PosCenter,PosPercent,0,0.35));
        //@deprecated
        //useText.setFillColor(SetAlpha(useText.getFillColor(),percentage(now,0,1600) * alpha));

         //Detect Events
        if(Mouse::isButtonPressed(Mouse::Left)){
            Vector2f pos = Vector2f(Mouse::getPosition(window).x,Mouse::getPosition(window).y);
            if(useText.getGlobalBounds().contains(pos)){
                al("The user clicked the engine support page.");
                system("start https://www.sfml-dev.org");
                timer.Pause();
                MessageBox(windowHwnd,"Thank you for going to the website of the game engine--sfml!","Thanks",MB_OK | MB_TOPMOST);
                timer.Resume();
            }
        }


        window.draw(useText);
        window.draw(logoText);
    }else{
        al("This scene's time is over.Switch to next scene.");
        ++sceneId;
    }
    return EXECUTE_SUC;
}

int DrawStates(RenderWindow & window){
    ///TODO : 下面这段什么意思？？？？
    //if(!focusing){
    //    Sleep(100);
    //    return EXECUTE_SUC;
    //}
    static sf::RenderTexture rt;
    static bool inited = false;
    static bool suc = false;
    static cck::Clock rsc;
    int returnResult = EXECUTE_SUC;

    if(rsc.Now().offset >= 500){
        rsc.GetOffset();
        museC.Update();
    }

    //Drawing Events
    window.clear(clearSceneColor);
    switch(sceneId){
        case SC_START_UP:{
            //start up cartoon
           returnResult = CartoonStartUp(window);
            break;
        }
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
            }
            if(suc){
                returnResult = mainMenuBackground(window,NULL,&rt);
                returnResult = returnResult | modsWindow(window,NULL,&rt);
            }else{
                returnResult = mainMenuBackground(window,NULL);
                returnResult = returnResult | modsWindow(window,NULL);
            }
            break;
        }
        case SC_WORLD:{
            ///内嵌调用mainMenuBackground
            returnResult = gameWindow(window);
            break;
        }
        case SC_SETTING:{
            if(!inited){
                inited = true;
                suc = rt.create(winSize.x,winSize.y);
            }
            if(suc){
                returnResult = mainMenuBackground(window,NULL,&rt);
                returnResult = returnResult | settingWindow(window);
            }else{
                returnResult = mainMenuBackground(window,NULL);
                returnResult = returnResult | settingWindow(window);
            }
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

//PlaneID:6
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
    static Sprite playerSp;
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
    window.draw(playerSp);

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
                    if(b){
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

#define CLOCK_ID 0x00060001
///gsc->bool 0:Sun   1:Moon
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
#undef CLOCK_ID

/*
**将单字节char*转化为宽字节wchar_t*
*/
inline wstring AnsiToUnicode(const char* szStr)
{
    int nLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0 );
    //if (nLen == 0)
    //{
    //    return wstring('\0');
    //}
    wchar_t* pResult = new wchar_t[nLen];
    MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen );
    wstring ret = pResult;
    free(pResult);
    return ret;
}

#define CLOCK_ID 0x00020001
int mainMenu(RenderWindow & window,GameSceneContacting * gsc){
    static bool musing = false;
    Vector2f pos(Mouse::getPosition(window).x,Mouse::getPosition(window).y);

    Text logoSp(t.Translate("game.defaultCaption","UnlimitedLife").GetUTF16(),*dfont,48);
    logoSp.setOutlineThickness(1);
    logoSp.setOutlineColor(Color::Black);

    FloatRect fr = logoSp.getGlobalBounds();
    logoSp.setPosition(setPosRelative(fr,winSize,PosCenter,PosPercent,0,0.15));

    window.draw(logoSp);

    if(!musing && rand()%100000 > 99900){
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
#undef CLOCK_ID

int loadingProc(RenderWindow & window){
    //static float loadRot = 0;
    //static float mod = ROTATING_PERCENT;
    //static float scale = 1;
    //static float smod = SCALING_PERCENT;
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
    ///Loading Processing///
    //Check Textures
    /*megaTexLFF(logoTexture,LOGO_PTH){
        EAssertEx(windowHwnd,"Cannot load logo image file！");
        return EXECUTE_FAI;
    }
    */
    clearSceneColor = Color::Black;


    ///Drawing Stuffs///

    Text logoSp("UnlimitedLife",*dfont,48);
    FloatRect fr = logoSp.getGlobalBounds();
    logoSp.setPosition(setPosRelative(fr,winSize,PosCenter,PosPercent,0,0.15));
    /*Sprite logoSp(logoTexture);
    scale += smod;
    logoSp.setScale(Vector2f(scale,scale));
    if(scale >= (1 + SC_MPEC) || scale < (1 - SC_MPEC))smod = -smod;
    FloatRect fr = logoSp.getGlobalBounds();
    logoSp.setPosition(setPosRelative(fr,winSize,PosPercent,PosPercent,0.5,0.2));
    logoSp.setOrigin(fr.width/2+fr.left,fr.height/2+fr.top);
    loadRot += mod;
    logoSp.setRotation(loadRot);
    if((int)abs(loadRot) >= RT_MPEC)mod = -mod;*/
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
                EAssertEx(windowHwnd,"There is something wrong in the mod loading process!")
            }else{
                al("Storing errors...");
                writer.write(loadProg.finalError.c_str(),loadProg.finalError.length());
                writer.close();
                string out = "There is something wrong in the mod loading process!Errors were stored in " + filePath;
                EAssertEx(windowHwnd,out.c_str());
            }
            if(loadProg.isCritical){
                al("Some of the errors are critical!Telling the user about this...");
                /*if(loadProg.loadKerFail){
                    al("The error is load the python kernel file fail.Check if files in res/api/ are exists and well-running!");
                    EAssertEx(windowHwnd,("Fail to load kernel python file \"" EX_ROOT "\n!This application can no longer support to load mods!!!"));
                }*/
                EAssertEx(windowHwnd,"This is a critical wrong!We suggest you to look at the log file!Or the game may crash easily!");
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
        //int yStep = 10;//MOD_UI_START_Y;
        //int xStep = 10;//MOD_UI_START_X;
        /*for(unsigned int i = 0;i < mh.data.size();++i){
            ModShow ms;
            Text nText(mh.data[i].config.modName,*dfont,12);
            nText.setPosition(xStep,yStep);
            nText.setFillColor(Color::Yellow);
            nText.setOutlineColor(Color::Yellow);
            FloatRect fr = nText.getLocalBounds();
            ms.modName = nText;
            nText.setString(mh.data[i].config.modAuthor);
            ms.modAut = nText;
            nText.setString(mh.data[i].config.modVersion);
            ms.modVer = nText;
            mods.push_back(ms);
            yStep += fr.top + fr.height;///TODO:fix this
            xStep = MOD_UI_START_X;
        }
        */
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
    /*
    string unZipData = "";
    //int numItems;
    //char * itemMem = NULL;
    //bool doLoadMtMod = true;
    //
    //Initializes read path
    */
    unsigned int decT = 0;
    string readPath = appData;
    readPath += folders[2];
    /*
    //Zip controls
    //HZIP zip;
    //ZIPENTRY ze;
    */

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
    sl(mo,"Mods Are:\n" + paths);
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

bool LogSaver::initStoring(string storeIn){
    outs(storeIn);
    m_writer.open(storeIn,ios::out | ios::trunc);
    m_inited = m_writer.is_open();
    return m_inited;
}
bool LogSaver::flush(){
    if(!m_inited)return false;
    m_buffer += "\n";
    m_writer.write(m_buffer.c_str(),m_buffer.length());
    m_writer.flush();
    m_buffer = "";
    return true;
}
bool LogSaver::close(){
    if(!m_inited)return false;
    this->flush();
    m_writer.close();
    m_inited = false;
    return true;
}
LogSaver::~LogSaver(){
    ///Close
    this->close();
}
void LogSaver::operator <<(string v){
    if(openedStoring){
        #ifdef LOG_AS_CON
        outn(v);
        #endif // LOG_AS_CON
        m_buffer += v;
    }
}
void LogSaver::operator <<(int v){(*this) << to_string(v);}
void LogSaver::operator <<(double v){(*this) << to_string(v);}
void LogSaver::operator <<(float v){(*this) << to_string(v);}
void LogSaver::operator <<(char * v){(*this) << string(v);}

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



MemTp GetCurrentMemoryUsage(){
    uint64_t mem = 0, vmem = 0;
    PROCESS_MEMORY_COUNTERS pmc;

    // get process hanlde by pid
    HANDLE process = GetCurrentProcess();
    if (GetProcessMemoryInfo(process, &pmc, sizeof(pmc)))
    {
        mem = pmc.WorkingSetSize;
        vmem = pmc.PagefileUsage;
    }
    CloseHandle(process);

    // use GetCurrentProcess() can get current process and no need to close handle

    // convert mem from B to MB
    return {(float)(mem / 1024.0 / 1024.0),(float)(vmem/1024.0/1024.0)};
}

GlMem GetGlobalMemoryUsage(){
    MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	DWORDLONG physical_memory = statex.ullTotalPhys / (1024 * 1024);
	DWORDLONG virtual_memory = (DWORD)statex.ullAvailVirtual / (1024 * 1024);
	DWORDLONG usePhys = physical_memory - (statex.ullAvailPhys / (1024 * 1024));

	float percent_memory = (float)usePhys / (float)physical_memory;
	return {percent_memory,(float)physical_memory,(float)virtual_memory,(float)usePhys};
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
