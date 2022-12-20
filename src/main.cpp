/**
自己选择了SFML,OpenGL,就别放弃！！！！！！！！！
不要为了一点难事而放弃！！！！！
加油！~(>o<)~!!!!
**/
//TODO:do not forget to change the debug io mode
#include "main.hpp"

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
map<int,cck::Clock> planeClocks;
HelperEvents he;

//Game Info
Font * dfont;
TexturesHelper texs;
Music bgm;
LogSaver ls;
GameManager gm;
Player player;
#include "dataStrs.h"
#include "@Game/TILES.h"
Register reg(gm);

//Mods
ModsHelper mh;

cck::Clock runningClock;

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

    sepl;

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
                #ifndef UNSTOP_WHEN_UNFOCUS
                focusing = false;
                if(bgm.getStatus() == Music::Playing)bgm.pause();
                timer.Pause();
                for(std::pair<const int,cck::Clock> & pairc : planeClocks){
                    pairc.second.Pause();
                }
                #endif // UNSTOP_WHEN_UNFOCUS
                if(reFrameLimitWhenUnFocus){
                    window.setFramerateLimit(24);
                    //al("Frame limited to 24fps...");
                }
            }else if(event.type == sf::Event::GainedFocus){
                #ifndef UNSTOP_WHEN_UNFOCUS
                focusing = true;
                if(bgm.getStatus() == Music::Paused)bgm.play();
                timer.Resume();
                for(std::pair<const int,cck::Clock> & pairc : planeClocks){
                    pairc.second.Resume();
                }
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
    if(!focusing){
        Sleep(100);
        return EXECUTE_SUC;
    }
    static sf::RenderTexture rt;
    static bool inited = false;
    static bool suc = false;
    int returnResult = EXECUTE_SUC;
    //一定要复用
    static GameSceneContacting gsc(2,false);//0:Sun   1:Moon
    gsc.clear();//gsc不会自动初始化
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
            returnResult = mainMenuBackground(window,&gsc);
            returnResult = returnResult | mainMenu(window,&gsc);
            break;
        }
        case SC_MODS:{
            if(!inited){
                inited = true;
                suc = rt.create(winSize.x,winSize.y);
            }
            if(suc){
                returnResult = mainMenuBackground(window,&gsc,&rt);
                returnResult = returnResult | modsWindow(window,&gsc,&rt);
            }else{
                returnResult = mainMenuBackground(window,&gsc);
                returnResult = returnResult | modsWindow(window,&gsc);
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
                returnResult = mainMenuBackground(window,&gsc,&rt);
                returnResult = returnResult | settingWindow(window);
            }else{
                returnResult = mainMenuBackground(window,&gsc);
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
    static vector<LayoutController> ls = {LayoutController()};
    static unsigned int cdx = 0;
    static bool test = false;
    ONLY_INIT_ONCE_INIT;
    clearSceneColor = Color(0,0,0);

    ONLY_INIT_ONCE_START
        Text * t = NULL;
        t = new Text(L"游戏的测试设置:" + wstring(test?L"开":L"关"),*dfont,24);
        ls[0].texts.push_back(t);
        ls[0].Set(0,0)->SetTextsAlign(ORI_CENTER)->SetTextPadding(8)->StaticForm(0,100,winSize.x,winSize.y);
    ONLY_INIT_ONCE_END

    if(he.mouseMov != -1){
        ls[cdx].PositionDetects(Vector2f(events[he.mouseMov].mouseMove.x,events[he.mouseMov].mouseMove.y),
        [&](Text * t,bool target){
            if(target){
                t->setFillColor(ColorMoreXX(white,0.8));
            }else{
                t->setFillColor(white);
            }
        });
    }else if(he.mouseRel != -1){
        Event& e = events[he.mouseRel];
        if(e.mouseButton.button == Mouse::Left){
            unsigned int id = ls[cdx].PositionDetects(Vector2f(e.mouseButton.x,e.mouseButton.y));
            switch(cdx){
            case 0:{
                switch(id){
                case 0:
                    test = !test;
                    ls[cdx][id].setString(L"游戏的测试设置:" + wstring(test?L"开":L"关"));
                    break;
                default:
                    break;
                }
                break;
            }
            default:
                break;
            }
        }
    }else if(he.keyPre != -1){
        Event e = events[he.keyPre];
        if(e.key.code == Keyboard::Escape){
            sceneId = SC_MENU;
        }
    }
    ls[cdx].draw(window);
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
        clearSceneColor = Color::Black;//Use black to fill the backgroud
        player.uuid_local = 114154;
        player.rSize = 1;
        player.position = Vector2f(0,0);
        player.dimension = 0;
        gm.Bind(player,window.getDefaultView());
        if(!tex.loadFromFile(PLAYER_BASE "pl_test.png"))exit(-1145142);
        playerSp.setPosition(gm.w /2 - tex.getSize().x/2, gm.h/2 - tex.getSize().y);
        playerSp.setTexture(tex);
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
        //Base movement events
        if(GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP)){
            player.Move(0,-0.05);
        }
        if(GetAsyncKeyState('S') || GetAsyncKeyState(VK_DOWN)){
            player.Move(0,0.05);
        }
        if(GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT)){
            player.Move(-0.05,0);
        }
        if(GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT)){
            player.Move(0.05,0);
        }
    }

    if(he.keyPre != -1){
        ExtractEvent(keyPre);
        if(MatchEKey(Keyboard::F3)){
            debugMode = !debugMode;
        }else if(MatchEKey(Keyboard::Escape)){
            sceneId = SC_MENU;
            ReInitEPI;
            gm.EndupGame();
        }
    }

    showFpsDB
    if(debugMode){
        sf::Text text(
            string("Unlimited Life F3 Debug v0.0:\n""Real Time:") + timeGt
                + "\nPlayer Position:" + VSTR_MAKE(player.position)
                + "\nPlayer Current Chunk Id:" + VSTR_MAKE(ChunkId(player.position))
                + "\nDimension Id:" + to_string(player.dimension)
                + "\nUUID Local:" + to_string(player.uuid_local)
        ,*dfont,16);
        text.setPosition(0,60);
        text.setFillColor(Color::Yellow);
        text.setOutlineColor(Color::White);
        window.draw(text);
    }
    return EXECUTE_SUC;
}

#define CLOCK_ID 0x00060001
///gsc->bool 0:Sun   1:Moon
int mainMenuBackground(RenderWindow & window,GameSceneContacting * gsc,RenderTexture * rt){
    ///Render the background
    ///Drawing Logo///
    static float bgPos = 0;
    static int sp0_ix = -800;
    static int sp1_ix = 0;
    static float rTheta = 0;
    static Vector2f sunPos(400,0);
    //static vector<Cloud> clouds;
    static vector<Sprite> clouds;
    static vector<Vector2f> stars;
    static Sprite basicStar;
    static cck::Clock clk;
    static cck::Clock shaderClk;
    static cck::Clock cloud;
    static float dec;
    //static RenderTexture clouds;
    //static RenderTexture noiseTex;
    [[maybe_unused]] static bool usingShader = false;
    static RenderTexture mb;
    static Shader rec;
    //static bool mouseSta = MS_NDOWN;
    //static bool sunMv = false;
    //static bool moonMv = false;
    static int flexibling = 0;
    static bool flexFirst = false;
    static Vector2f oldPos;
    static Shader shader;
    ONLY_INIT_ONCE_INIT;
    Vector2f pos(Mouse::getPosition(window).x,Mouse::getPosition(window).y);

    ONLY_INIT_ONCE_START
        for(int i = 0;i < rand()%25+26;i++){
            stars.push_back(Vector2f((float)(rand() % 800),(float)(rand() % 600)));
        }
        basicStar.setTexture(*texs["star"]);
        FloatRect fr = basicStar.getGlobalBounds();
        basicStar.setOrigin(fr.width/2+fr.left,fr.height/2+fr.top);
        planeClocks.insert(std::make_pair(CLOCK_ID,cck::Clock()));
        if(!mb.create(window.getSize().x,window.getSize().y)){
            al("Cannot create buffer!At main menu!");
            EAssertExEx(window.getSystemHandle(),"Unable to create a buffer to draw images!","Error");
            exit(-1);
        }
        if(shaderStatus.isAvailable){
            if(rec.loadFromFile(shaders[SHADER_REC],Shader::Fragment)){
                //if(clouds.create(winSize.x,winSize.y)){
                        usingShader = true;
                //}
            }
        }
    if(usingShader){
        /*noiseTex.create(winSize.x,winSize.y);
        Sprite v = ml(Sprite(*texs["cloud7"]),v.setPosition(100,100););
        noiseTex.draw(v);
        used4Clouds.setUniform("cloudTex",noiseTex.getTexture());*/
        rec.setUniform("textureR", sf::Shader::CurrentTexture);
    }
    ONLY_INIT_ONCE_END

    float deltaTime = planeClocks[CLOCK_ID].GetOffset();
    float deltaMove = deltaTime / 18;

    bgPos += BACK_MOVE * deltaMove;
    ///Drawing Scrolling Back Ground Image///
    Sprite sp0(*texs[nameTexs[0]]);
    Sprite sp1(*texs[nameTexs[1]]);

    ///Swap Position///
    if(bgPos > 800){
        bgPos = 0;
        float tempsp = sp0_ix;
        sp0_ix = sp1_ix;
        sp1_ix = tempsp;
    }
    sp0.setPosition(sp0_ix,0);
    sp1.setPosition(sp1_ix,0);
    sp0.move(bgPos,0);
    sp1.move(bgPos,0);
    ///Drawing The Sun
    if(!gsc->bools[0] && !gsc->bools[1]){
        rTheta += (SUN_MOON_RT * deltaMove);

        if(rTheta >= 360){
            rTheta = 0;
        }
    }

    ONLY_INIT_ONCE_INIT_I(1);
    ONLY_INIT_ONCE_START_I(1)
        rTheta = 140;
        /**
        issue:不知为何,游戏第一帧的deltaTime特别大，因此，须在此处再次赋值
        */
    ONLY_INIT_ONCE_END

    Sprite sun(*texs["sun"]);
    Sprite moon(*texs["moon"]);
    FloatRect fr = sun.getGlobalBounds();
    sun.setOrigin(fr.width/2+fr.left,fr.height/2+fr.top);
    ///Set Dragging Flexible Moving
    if(flexibling != FLEX_SUN){
        sun.setPosition(RotateAround(deg2rad(rTheta),Vector2f(0,200),Vector2f(400,600)));
    }else if(!flexFirst){
        flexFirst = true;
        sun.setPosition(pos);
    }else{
        sun.setPosition(oldPos);
    }
    fr = moon.getGlobalBounds();
    moon.setOrigin(fr.width/2+fr.left,fr.height/2+fr.top);
    if(flexibling != FLEX_MOON){
        moon.setPosition(RotateAround(deg2rad(rTheta+180),Vector2f(0,200),Vector2f(400,600)));
    }else if(!flexFirst){
        flexFirst = true;
        moon.setPosition(pos);
    }else{
        moon.setPosition(oldPos);
    }
    if(clk.GetALLTime() > 5000 && dec > 0.6){
        clk.Stop();
        clk.Start();
        stars.clear();
        for(int i = 0;i < rand()%25+26;i++){
            stars.push_back(Vector2f((float)(rand() % 800),(float)(rand() % 600)));
        }
    }

    block(Drawing Sun Moon & Stars){
        dec = sin(deg2rad(rTheta / 2));
        clearSceneColor =  Color(160*dec,160*dec,255*dec);
        mb.clear(clearSceneColor);
        if(cloud.checkEslapse(200) && clouds.size() < 16 && rand() % 10000 > 9970 ){
            string openC = "cloud" + to_string(rand() % CLOUD_C);
            cloud.GetOffset();
            Sprite cloud(*texs[openC]);
            cloud.setPosition(0,0);
            cloud.setPosition(-cloud.getLocalBounds().width - 10,rand()%600);
            cloud.setRotation(1);
            ///Beautiful Sun Set & Rise
            clouds.push_back(cloud);
        }

        if(dec < 0.5){
            ///Show Stars
            basicStar.setColor(Color(255,255,255,(int)((1 - percentage(dec,0,0.5))*255)));
            for(const Vector2f & pos : stars){
                if(rand() % 10000 > 9960)continue;//Shining Code
                basicStar.setPosition(pos);
                mb.draw(basicStar);
            }
        }
        //Sun move
        if(gsc->bools[0]){
            float ag = getAngleSigned(pos-Vector2f(400,600),Vector2f(0,-1));
            float percent = (ag + 90)/180;
            rTheta = percent * 180 + 90;
            sun.setPosition(pos);
        }
        //Moon move
        if(gsc->bools[1]){
            float ag = -getAngleSigned(pos-Vector2f(400,600),Vector2f(0,1));
            float percent = (ag+90)/180;
            rTheta = percent * 180 + 90;
            moon.setPosition(pos);
        }
        if(flexibling == FLEX_SUN){
            Vector2f expectPos = RotateAround(deg2rad(rTheta),Vector2f(0,200),Vector2f(400,600));
            Vector2f currentPos = sun.getPosition();
            Vector2f dct = expectPos - currentPos;
            if(Length(dct) <= 0.5){
                ///Now,the difference is very small
                flexibling = 0;
                flexFirst = false;
            }else{
                oldPos = currentPos + Normalize(dct)*FLEX_SPEED*deltaMove;
                sun.setPosition(oldPos);
            }
        }else if(flexibling == FLEX_MOON){//吐槽：明明代码可复用程度很高，但是我太懒了！
            Vector2f expectPos = RotateAround(deg2rad(rTheta+180),Vector2f(0,200),Vector2f(400,600));
            Vector2f currentPos = moon.getPosition();
            Vector2f dct = expectPos - currentPos;
            if(Length(dct) <= 0.5){
                ///Now,the difference is very small
                flexibling = 0;
                flexFirst = false;
            }else{
                oldPos = currentPos + Normalize(dct)*FLEX_SPEED*deltaMove;
                moon.setPosition(oldPos);
            }
        }
        mb.draw(sun);
        mb.draw(moon);
        //clouds.clear(Color(255,255,255,128));
        //if(usingShader)mb.draw(Sprite(clouds.getTexture()),&used4Clouds);
        //else mb.draw(Sprite(clouds.getTexture()));
        for(Sprite & perC : clouds){
            perC.move(MV_CLD*deltaMove,rand()%1000 > 900?(rand()%1000 > 900 ? (MV_CLD*deltaMove) : -(MV_CLD*deltaMove)):0);
            perC.setColor(Color(255*dec,255*dec,255*dec));
            mb.draw(perC);
        }
        {
            static cck::Clock priv_clock;
            if(priv_clock.Now().offset >= 1000){
                priv_clock.GetOffset();
                for(unsigned int i = 0;i < clouds.size();){
                    if(clouds[i].getPosition().x >= 800){
                        clouds.erase(clouds.begin() + i);
                        continue;
                    }
                    i++;
                }
            }
        }
    }

    ///Drawing Stuff
    sp0.setColor(ColorMoreXX(Color(255,255,255),(dec + 0.1) > 1 ? 1 : dec + 0.1));
    sp1.setColor(ColorMoreXX(Color(255,255,255),(dec + 0.1) > 1 ? 1 : dec + 0.1));
    mb.draw(sp0);
    mb.draw(sp1);
    Sprite sp(mb.getTexture());
    sp.scale(1,-1);
    sp.setPosition(0,mb.getSize().y);
    rec.setUniform("time",(int)shaderClk.GetALLTime());
    window.draw(sp,&rec);
    if(rt)rt->draw(sp,&rec);

    ///Detect Dragging
    if(he.mousePre != -1){
        //mouseSta = MS_DOWN;
        if(sun.getGlobalBounds().contains(pos)){
            gsc->bools[0] = true;
        }
        if(moon.getGlobalBounds().contains(pos)){
            gsc->bools[1] = true;
        }
    }
    if(he.mouseRel != -1){
        //mouseSta = MS_NDOWN;
        if(gsc->bools[0]){
            gsc->bools[0] = false;
            if(rTheta >= 90 && rTheta <= 270)flexibling = FLEX_SUN;
        }
        if(gsc->bools[1]){
            gsc->bools[1] = false;
            if((rTheta < 90 && rTheta >= 0) || (rTheta > 270 && rTheta <= 360))flexibling = FLEX_MOON;
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
    ///Drawing Logo///
    //static float loadRot = 0;
    //static float mod = ROTATING_PERCENT;
    //static float scale = 1;
    //static float smod = SCALING_PERCENT;
    //static int rdi = 0;
    static Vector2f sunPos(400,0);
    static vector<Sprite> clouds;
    static vector<Vector2f> stars;
    static Sprite basicStar;
    static cck::Clock clk;
    //static unsigned short times = 0;
    [[maybe_unused]] static int changeVl = 0;
    static Vector2f oldPos;
    static int muIdx = randRange(MENU_MSC_RANGE_MIN,MENU_MSC_RANGE_MAX);
    static double countTime = 0;
    [[maybe_unused]] initEPI;
    ONLY_INIT_ONCE_INIT;
    Vector2f pos(Mouse::getPosition(window).x,Mouse::getPosition(window).y);

    /*enterPerInit
        rdi = rand() % showingStrings.size();
    endEPI
    */

    /*
        这里不要写clearSceneColor!!!
        否则背景为黑！！！
    */

    ONLY_INIT_ONCE_START
        sepl;
        planeClocks.insert(std::make_pair(CLOCK_ID,cck::Clock()));
        al("Main menu first initialized.");
        if(MENU_MSC_RANGE_MIN >= 0){
            bgm.stop();
            bgm.openFromFile(musics[muIdx]);
            bgm.setLoop(false);
            bgm.play();
        }
    ONLY_INIT_ONCE_END

    countTime += planeClocks[CLOCK_ID].GetOffset();

    #if MENU_MSC_RANGE_MIN >= 0
            if(bgm.getStatus() == Music::Stopped || changeVl){
                al("The background music changed...");
                if(!changeVl){
                    muIdx++;
                    if(muIdx > MENU_MSC_RANGE_MAX){
                        muIdx = MENU_MSC_RANGE_MIN;
                    }
                }else{
                    muIdx += changeVl;
                    if(muIdx > MENU_MSC_RANGE_MAX){
                        muIdx = MENU_MSC_RANGE_MIN;
                    }else if(muIdx < MENU_MSC_RANGE_MIN){
                        muIdx = MENU_MSC_RANGE_MAX;
                    }
                    changeVl = 0;
                }

                al("Now the music is \"" + musics[muIdx] +"\"");

                bgm.stop();
                bgm.setVolume(100);
                bgm.openFromFile(musics[muIdx]);
                bgm.setLoop(false);
                bgm.play();
            }
    #endif // MENU_MSC_RANGE_MIN

    //Sprite logoSp(logoTexture);
    Text logoSp("UnlimitedLife",*dfont,48);
    /*if(countTime >= 5){
        countTime = 0;
        scale += smod;
        loadRot += mod;
    }*/
    //logoSp.setScale(Vector2f(LOGO_TEXT_SCALE,LOGO_TEXT_SCALE));
    FloatRect fr = logoSp.getGlobalBounds();
    logoSp.setPosition(setPosRelative(fr,winSize,PosCenter,PosPercent,0,0.15));
    /*if(scale >= (1 + SC_MPEC) || scale < (1 - SC_MPEC))smod = -smod;
    //logoSp.setOrigin(fr.width/2+fr.left,fr.height/2+fr.top);
    //logoSp.setRotation(loadRot);
    //if((int)abs(loadRot) >= RT_MPEC)mod = -mod;
    */
    /*
    ///Yellow Text///
    string sv = "";
    //Special
    switch(times){
    default:
        sv = showingStrings[rdi];
        break;
    case 20:
        sv = "You clicked me so many times!";
        break;
    case 30:
        sv = "Awww!>_<";
        break;
    case 40:
        sv = "Fuck you!";
        break;
    }
    Text shows(sv,*dfont,20);

    //show Text set
    shows.setPosition(0,0);
    shows.setOutlineColor(Color::White);
    shows.setFillColor(Color::White);
    Vector2f odr = Vector2f(fr.width/512 * 255,fr.height/512 * 255);
    fr = shows.getGlobalBounds();
    {
        float calcScale = 0;
        if(showingStrings[rdi].length() >= 10){
            calcScale = (showingStrings[rdi].length() - 10) * 0.01;
            if(calcScale >= 1){
                calcScale = 0.9;
            }
        }
        shows.setScale(Vector2f(1.0-calcScale,1.0-calcScale));
    }
    shows.setPosition(logoSp.getPosition()+odr);
    shows.setOrigin(fr.width/2+fr.left,fr.height/2+fr.top);
    shows.setFillColor(Color::Yellow);
    shows.setOutlineColor(Color::Yellow);
    shows.setRotation(loadRot - 30);
    */

    window.draw(logoSp);
    //window.draw(shows);

    ///Drawing Main Menu
    block(Drawing Main Menu){
        Text StartGame("Start Game",*dfont,28);
        Text Mods("Mod List",*dfont,28);
        Text Settings("Settings",*dfont,28);
        Text QuitGame("Exit",*dfont,28);
        Color pmc = white;//Public Menu Color

        StartGame.setOutlineColor(pmc);
        StartGame.setFillColor(pmc);
        StartGame.setPosition(setPosRelative(StartGame.getLocalBounds(),winSize,PosPercent,PosPercent,0.1,0.5));

        Mods.setOutlineColor(pmc);
        Mods.setFillColor(pmc);
        Mods.setPosition(setPosRelative(Mods.getLocalBounds(),winSize,PosPercent,PosPercent,0.1,0.6));

        Settings.setOutlineColor(pmc);
        Settings.setFillColor(pmc);
        Settings.setPosition(setPosRelative(Settings.getLocalBounds(),winSize,PosPercent,PosPercent,0.1,0.7));

        QuitGame.setOutlineColor(pmc);
        QuitGame.setFillColor(pmc);
        QuitGame.setPosition(setPosRelative(QuitGame.getLocalBounds(),winSize,PosPercent,PosPercent,0.1,0.8));

        if(he.mouseRel != -1 && !gsc->bools[0] && !gsc->bools[1]){//0:Sun   1:Moon
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
            ReInitEPI;
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

    /*if(he.mouseRel != -1){
        if(shows.getGlobalBounds().contains(pos)){
            times++;
            rdi = rand() % showingStrings.size();
        }
    }*/
    if(he.keyRel != -1){
        Event::KeyEvent ke = events[he.keyRel].key;
        if(ke.code == Keyboard::Left && ke.control){
            changeVl = -1;
        }else if(ke.code == Keyboard::Right && ke.control){
            changeVl = 1;
        }
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
            window.setTitle("UnlimitedLife");
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


    Text back2MainMenu("Back",*dfont,24);
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
                    sl(mo,"Mod" + mod.info.name + " failed when loading!!!!See the Mod;s Log for more detail...");
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
