//TODO:do not forget to change the debug io mode
#include "main.hpp"

using namespace std;
using namespace sf;
using namespace cck;
using namespace rapidjson;

//This is the id of the current scene
int sceneId = checkDebug(1,0);//If debug,skip the first scene
cck::Clock timer = cck::Clock(false);
DWORD startTime;

//Lock
PyLock pylock;

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
Texture logoTexture;
TexturesHelper texs;
Music bgm;
LogSaver ls;
#include "dataStrs.h"

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

#define al addLog
#define sepl ls << string("-----------------------------------------------------\n")
#define ssep ls << string("*****************************************************\n")

#define ssil(x) ssep;il(x);ssep

int main(){
    srand(time(0));
    al("Starting this application...");
    al("Loading font...");
    if(MegaFont::loadDefault()){
        al("Error:default font missing...");
        EAssert("Fail to load default font!Maybe you should re-install this application!");
        return -1;
    }
    al("Initializing mod executer,Python...");
    if(pylock.init() != EXECUTE_SUC){
        al("Error:Python initializes fail!");
        EAssert("Python inited fail!!!!!This application now cannot load mods!!!");
        modLoadingGood = false;
    }
    al("Initializes Python successfully!Now are building fonts...");
    dfont = MegaFont::getDefaultFont();
    //Font Data
    //Mistake 0:不要用=复制进行初始化，要用如下初始化，析构函数会删掉加载的dfont
    //Mistake 1:一定要对default font 与normal font区别处理！否则会因为析构函数重复删去内存导致0xC0000005(内存问题)
    MegaFont defaultFontMega(dfont,true,true);
    al("Loading extra system fonts...");
    LoadFonts();
    al("Creating main windows...");
    //Create Main Window
    RenderWindow window(sf::VideoMode(winSize.x,winSize.y),"MineTerrariaModLoader",Style::Titlebar | Style::Close);
    windowHwnd = window.getSystemHandle();
    al("Restrict update frame limit...");
    if(RESTRICT_FRAME_LIMIT >= 0)window.setFramerateLimit(RESTRICT_FRAME_LIMIT);//Set frame limit to 120 fps

    sepl;
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
                static bool showed_bxx = false;///Special id
                if(!showed_bxx){
                    showed_bxx = true;
                    il("The user tried to close the window directly.While we do not allow him/her to do that.(*/ω＼*)");
                }
                ///Do Nothing
            }else if(event.type == sf::Event::LostFocus){
                focusing = false;
                if(bgm.getStatus() == Music::Playing)bgm.pause();
                timer.Pause();
            }else if(event.type == sf::Event::GainedFocus){
                focusing = true;
                if(bgm.getStatus() == Music::Paused)bgm.play();
                timer.Resume();
            }else{
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
    if(crangeEq(offset,0,2400)){

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
    int returnResult = EXECUTE_SUC;
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
            returnResult = mainMenu(window);
            break;
        }
        case SC_MODS:{
            returnResult = modsWindow(window);
        }
        default:
            break;
    }
    ///Maybe TODO: move it to draw function,because drawable will be destroy
    //Display The Window
    window.display();
    return returnResult;
}

int mainMenu(RenderWindow & window){
    ///Drawing Logo///
    static float loadRot = 0;
    static float mod = ROTATING_PERCENT;
    static float scale = 1;
    static float smod = SCALING_PERCENT;
    static bool inited = false;
    static float bgPos = 0;
    static int sp0_ix = -800;
    static int sp1_ix = 0;
    static int rdi = 0;
    static float rTheta = 140;
    static Vector2f sunPos(400,0);
    static vector<Sprite> clouds;
    static vector<Vector2f> stars;
    static Sprite basicStar;
    static cck::Clock clk;
    static float dec;
    //static bool mouseSta = MS_NDOWN;
    static bool sunMv = false;
    static bool moonMv = false;
    static int flexibling = 0;
    static bool flexFirst = false;
    static int changeVl = 0;
    static Vector2f oldPos;
    static int muIdx = randRange(MENU_MSC_RANGE_MIN,MENU_MSC_RANGE_MAX);
    initEPI;
    Vector2f pos(Mouse::getPosition(window).x,Mouse::getPosition(window).y);

    clearSceneColor = Color::Black;

    enterPerInit
        rdi = rand() % showingStrings.size();
    endEPI

    if(!inited){
        sepl;
        al("Main menu first initialized.");
        inited = true;
        bgm.stop();
        bgm.openFromFile(musics[muIdx]);
        bgm.setLoop(false);
        bgm.play();
        for(int i = 0;i < rand()%25+26;i++){
            stars.push_back(Vector2f((float)(rand() % 800),(float)(rand() % 600)));
        }
        basicStar.setTexture(*texs["star"]);
        FloatRect fr = basicStar.getGlobalBounds();
        basicStar.setOrigin(fr.width/2+fr.left,fr.height/2+fr.top);
    }
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
        bgm.openFromFile(musics[muIdx]);
        bgm.setLoop(false);
        bgm.play();
    }
    bgPos += BACK_MOVE;
    Sprite logoSp(logoTexture);
    scale += smod;
    logoSp.setScale(Vector2f(scale,scale));
    if(scale >= (1 + SC_MPEC) || scale < (1 - SC_MPEC))smod = -smod;
    FloatRect fr = logoSp.getGlobalBounds();
    logoSp.setPosition(setPosRelative(fr,winSize,PosPercent,PosPercent,0.5,0.15));
    logoSp.setOrigin(fr.width/2+fr.left,fr.height/2+fr.top);
    loadRot += mod;
    logoSp.setRotation(loadRot);
    if((int)abs(loadRot) >= RT_MPEC)mod = -mod;
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

    ///Yellow Text///
    Text shows(showingStrings[rdi],*dfont,20);

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

    ///Drawing The Sun

    if(!sunMv && !moonMv){
        rTheta += SUN_MOON_RT;

        if(rTheta >= 360){
            rTheta = 0;
        }
    }

    Sprite sun(*texs["sun"]);
    Sprite moon(*texs["moon"]);
    fr = sun.getGlobalBounds();
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
        if(clouds.size() < 50 && rand() % 10000 > 9940 ){
            string openC = "cloud" + to_string(rand() % CLOUD_C);
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
                window.draw(basicStar);
            }
        }
        if(sunMv){
            float ag = getAngleSigned(pos-Vector2f(400,600),Vector2f(0,-1));
            float percent = (ag + 90)/180;
            rTheta = percent * 180 + 90;
            sun.setPosition(pos);
        }
        if(moonMv){
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
                oldPos = currentPos + Normalize(dct)*FLEX_SPEED;
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
                oldPos = currentPos + Normalize(dct)*FLEX_SPEED;
                moon.setPosition(oldPos);
            }
        }
        window.draw(sun);
        window.draw(moon);
        for(Sprite & perC : clouds){
            perC.move(MV_CLD,rand()%1000 > 900?(rand()%1000 > 900 ? MV_CLD : -MV_CLD):0);
            perC.setColor(Color(255*dec,255*dec,255*dec));
            window.draw(perC);
        }
        for(unsigned int i = 0;i < clouds.size();){
            if(clouds[i].getPosition().x >= 800){
                clouds.erase(clouds.begin() + i);
                continue;
            }
            i++;
        }
    }

    ///Drawing Stuff
    sp0.setColor(ColorMoreXX(Color(255,255,255),(dec + 0.1) > 1 ? 1 : dec + 0.1));
    sp1.setColor(ColorMoreXX(Color(255,255,255),(dec + 0.1) > 1 ? 1 : dec + 0.1));
    window.draw(sp0);
    window.draw(sp1);
    window.draw(logoSp);
    window.draw(shows);

    ///Drawing Main Menu
    block(Drawing Main Menu){
        Text StartGame("Start Game",*dfont,28);
        Text Mods("Mod List",*dfont,28);
        Text Settings("Settings",*dfont,28);
        Text QuitGame("Exit",*dfont,28);
        Color pmc = dirt;//Public Menu Color

        StartGame.setOutlineColor(pmc);
        StartGame.setFillColor(pmc);
        StartGame.setPosition(setPosRelative(StartGame.getLocalBounds(),winSize,PosCenter,PosPercent,0,0.5));

        Mods.setOutlineColor(pmc);
        Mods.setFillColor(pmc);
        Mods.setPosition(setPosRelative(Mods.getLocalBounds(),winSize,PosCenter,PosPercent,0,0.6));

        Settings.setOutlineColor(pmc);
        Settings.setFillColor(pmc);
        Settings.setPosition(setPosRelative(Settings.getLocalBounds(),winSize,PosCenter,PosPercent,0,0.7));

        QuitGame.setOutlineColor(pmc);
        QuitGame.setFillColor(pmc);
        QuitGame.setPosition(setPosRelative(QuitGame.getLocalBounds(),winSize,PosCenter,PosPercent,0,0.8));

        if(he.mouseRel != -1 && !sunMv && !moonMv){
            if(QuitGame.getGlobalBounds().contains(pos)){
                ///Store Logs When The Application Quits
                ls.close();
                window.close();
            }else if(StartGame.getGlobalBounds().contains(pos)){
                EAssert("Not developed yet!");
            }else if(Settings.getGlobalBounds().contains(pos)){
                EAssert("Not developed yet!");
            }else if(Mods.getGlobalBounds().contains(pos)){
                sceneId = SC_MODS;
                ReInitEPI;
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

    ///Detect Dragging
    if(he.mousePre != -1){
        //mouseSta = MS_DOWN;
        if(sun.getGlobalBounds().contains(pos)){
            sunMv = true;
        }
        if(moon.getGlobalBounds().contains(pos)){
            moonMv = true;
        }
    }
    if(he.mouseRel != -1){
        //mouseSta = MS_NDOWN;
        if(sunMv){
            sunMv = false;
            if(rTheta >= 90 && rTheta <= 270)flexibling = FLEX_SUN;
        }
        if(moonMv){
            moonMv = false;
            if((rTheta < 90 && rTheta >= 0) || (rTheta > 270 && rTheta <= 360))flexibling = FLEX_MOON;
        }
        if(shows.getGlobalBounds().contains(pos)){
            rdi = rand() % showingStrings.size();
        }
    }
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

int loadingProc(RenderWindow & window){
    static float loadRot = 0;
    static float mod = ROTATING_PERCENT;
    static float scale = 1;
    static float smod = SCALING_PERCENT;
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
    megaTexLFF(logoTexture,LOGO_PTH){
        EAssertEx(windowHwnd,"Cannot load logo image file！");
        return EXECUTE_FAI;
    }
    clearSceneColor = Color::Black;


    ///Drawing Stuffs///
    Sprite logoSp(logoTexture);
    scale += smod;
    logoSp.setScale(Vector2f(scale,scale));
    if(scale >= (1 + SC_MPEC) || scale < (1 - SC_MPEC))smod = -smod;
    FloatRect fr = logoSp.getGlobalBounds();
    logoSp.setPosition(setPosRelative(fr,winSize,PosPercent,PosPercent,0.5,0.2));
    logoSp.setOrigin(fr.width/2+fr.left,fr.height/2+fr.top);
    loadRot += mod;
    logoSp.setRotation(loadRot);
    if((int)abs(loadRot) >= RT_MPEC)mod = -mod;
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
                if(loadProg.loadKerFail){
                    al("The error is load the python kernel file fail.Check if files in res/api/ are exists and well-running!");
                    EAssertEx(windowHwnd,"Fail to load kernel python file \"" EX_ROOT "\n!This application can no longer support to load mods!!!");
                }
                EAssertEx(windowHwnd,"This is a critical wrong!We suggest you to look at the log file!Or the game may crash easily!");
            }
        }
    }
    return EXECUTE_SUC;
}

int modsWindow(RenderWindow & window){
    static vector<ModShow> mods;
    static bool inited = false;
    static int rand_num = rand() % 2;
    Vector2f pos(Mouse::getPosition(window).x,Mouse::getPosition(window).y);
    clearSceneColor = Color(180,180,255);
    if(!inited){
        inited = true;
        int yStep = MOD_UI_START_Y;
        int xStep = MOD_UI_START_X;
        for(unsigned int i = 0;i < mh.data.size();++i){
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
    }
    for(ModShow & msc : mods){
        window.draw(msc.modName);
    }

    Text back2MainMenu("Back",*dfont,24);
    back2MainMenu.setFillColor(Color::White);
    back2MainMenu.setPosition(setPosRelative(back2MainMenu.getLocalBounds(),winSize,PosCenter,PosPercent,0,0.95));

    if(he.mouseRel != -1){
        if(back2MainMenu.getGlobalBounds().contains(pos)){
            sceneId = SC_MENU;
        }
    }

    window.draw(Sprite(*texs[nameTexs[rand_num]]));
    window.draw(back2MainMenu);

    showFpsDB

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
    string unZipData = "";
    int numItems;
    char * itemMem = NULL;
    bool doLoadMtMod = true;
    unsigned int decT = 0;
    //Initializes read path
    string readPath = appData;
    readPath += "\\StudyAll\\MineTerraria\\Mods";
    //Zip controls
    HZIP zip;
    ZIPENTRY ze;

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

    ///First Mod Step:Reading
    if(modLoadingGood){
        al("Reading kernel python file...");
        ///Reading Kernel Data
        {
            setStr("Reading Kernel Data...");
            ifstream kernelApi;
            kernelApi.open(EX_ROOT);
            if(kernelApi.is_open()){
                string file_ker = fileIO::readAll(kernelApi);
                kernelApi.close();
                if(initModRootFile(file_ker) != EXECUTE_SUC){
                    al("Fail to load kernel python file!");
                    invokeOne(timeGt + "Fail to load kernel python file \"" EX_ROOT "\n!This application can no longer support to load mods!!!" );
                    lp->isCritical = true;
                    lp->loadKerFail = true;
                    doLoadMtMod = false;
                    return NULL;
                }
            }else{
                al("Fail to open kernel python file!");
                invokeOne(timeGt + "Fail to open kernel python file \"" EX_ROOT "\n!This application can no longer support to load mods!!!" );
                lp->isCritical = true;
                lp->loadKerFail = true;
                doLoadMtMod = false;
                return NULL;
            }
            al("Initializing kernel file data");
            setStr("Initializing mod data...");
            PyObject_ModModified m_pm = GetPm();
            PyObject * strs = m_pm.menuStringsPointer;
            apStringLists(showingStrings,strs);
        }
        ///Loading Mt Mods
        if(doLoadMtMod){
            al("Loading personal mods...");
            setStr("Read Mods List...");
            getFileNames(readPath,paths);
            //Delete un-useful packs
            while(decT < paths.size()){
                //Has Differrent
                if(paths[decT].substr(paths[decT].find_last_of('.') + 1).compare("mtmod") && paths[decT].substr(paths[decT].find_last_of('.') + 1).compare("manifest")){
                    al(paths[decT] + " do not fits the suffix \".mtmod\"(Zipped) or \".manifest\"(Unzipped)");
                    paths.erase(paths.begin() + decT);
                    continue;
                }
                ++decT;
            }
            al("Reading mods's data...");
            //Get Mods
            for(unsigned int i = 0;i < paths.size();i++){
                vector<SingalModData> items;
                ModConfig mcg;
                bool canTakeIn = false;
                bool modSame = false;
                sepl;
                if(!paths[i].substr(paths[i].find_last_of('.') + 1).compare("manifest")){
                    al("One mod uses unzipped data while we do not support this now...");
                    ///Fix This..
                }else{
                    al("Reading zip:" + paths[i]);
                    //Read Zip File
                    zip = OpenZip(paths[i].c_str(),0);
                    if(!zip){
                        al("Opened zip file fail!");
                        invokeOne(timeGt + "Cannot open or understand mod file,maybe it is not a zip file:" + paths[i] + "\n" NT_BAD_MOD);
                        continue;
                    }
                    al("Opened zip file successfully.Now are getting sizes...");
                    GetZipItem(zip,-1,&ze);
                    numItems = ze.index;
                    string::size_type iPos = paths[i].find_last_of('\\') + 1;
                    string modName = paths[i].substr(iPos,paths[i].length() - iPos);
                    if(modName.length() > 16){
                        modName = modName.substr(0,13) + "...";
                    }
                    for (int pi=0; pi< numItems; pi++){
                        //多次的提醒:不加float会取整!!!
                        GetZipItem(zip,pi,&ze);
                        ///TODO:Change the loading string to the mod package name,not the file name
                        setStr("Read Mods List:" + modName + " "
                               + to_string(i) + "/" + to_string(paths.size()) + " " + to_string(pi) + "/" + to_string(numItems));
                        itemMem = (char*)malloc(sizeof(char) * (ze.unc_size+1));
                        if(!itemMem){
                            invokeAnError(timeGt + "Fail to allocate the memory when load mod file\"" + paths[i] + "  " + string(ze.name) + "\",maybe restart this application can fix this problem." NT_CANNNOT_ALLOC);
                            break;
                        }
                        memset(itemMem,0,sizeof(char) * (ze.unc_size+1));
                        UnzipItem(zip,pi,(void *)itemMem,ze.unc_size);
                        unZipData = itemMem;
                        items.push_back(SingalModData(unZipData,string(ze.name)));
                        free(itemMem);
                    }
                    al("Checking mod data...");
                    if(!ModDataCheckData(items,MOD_MANIFEST_PATH)){
                        al(string("This mod does not have manifest file called \"./" MOD_MANIFEST_PATH "\"!"));
                        invokeOne(timeGt + "Fail to load mod \"" + paths[i] + "\" because the \"mod\" seems not like a real mod!Maybe the mod file is corrupted!" NT_BAD_MOD);
                    }else{
                        al("Reading mod manifest file as a configuration...");
                        int status = readModConfig(ModDataFindData(items,MOD_MANIFEST_PATH)->data,mcg);
                        al("Reading finished:here is the result:" + readStatus(status));
                        if(status != EXECUTE_SUC){
                            invokeOne(timeGt + "Fail to load mod \"" + paths[i] + "\" because the mod's manifest file is not a manifest file!Maybe the mod file is corrupted!" NT_BAD_MOD);
                        }else{
                            al("Detect if the mod's id is not unknown...");
                            ///Detect for package
                            if(!mcg.modPack.compare(UNKNOWN_MOD_NAME)){
                                ///TODO:Add this log a website to know how to define a mod
                                al("The mod\"" + paths[i] + string("\" does not has a manifest item named \"" MOD_ID_NAME "\"!Please add it if you are this mod's developer!"));
                                invokeOne(timeGt + "Fail to load mod \"" + paths[i] + "\" because the mod's package name is unknown!Maybe the mod file is corrupted!" NT_BAD_MOD);
                            }else{
                                al("Detecting same mod with different name...");
                                ///Detect The Same Mod
                                for(SingalModFile & smf : mh.data){
                                    if(!smf.config.modPack.compare(mcg.modPack)){
                                        ///The same mod!
                                        al("Discovered the same mod here:\"" + paths[i] + "\"");
                                        outn("discovered same mod!" + paths[i]);
                                        invokeOne(timeGt + "Fail to load mod \"" + paths[i] + "\"version of " + mcg.modVersion + "because the mod is same to the mod \"" + smf.loadFileName + "\" version of " + smf.config.modVersion + NT_SAME_MOD);
                                        modSame = true;
                                    }
                                }
                                if(!modSame){
                                    canTakeIn = true;
                                }
                            }
                        }
                    }
                    CloseZip(zip);
                }
                if(canTakeIn){
                    al("This mod loaded successfully!Adding it to mod list...");
                    ModFile_Struct mfs;
                    ///Get Mod Main File
                    for(SingalModData & smd : items){
                        if(!smd.path.compare(mcg.script_main)){
                            readModData(smd.data,"modLoaded" + to_string(i),mfs);
                        }
                    }
                    mh.data.push_back(SingalModFile(mcg,items,paths[i],mfs));
                }
                sepl;
            }
        }
    }
    ///Second Mod Step:Initializing
    if(modLoadingGood && doLoadMtMod){
        al("Initializing mods....");
        setStr("Initializing Mods...");
        PyRun_SimpleString("from mtmod_loader import *\nmlc." LOADING_PROC_ITEM " = 0");
        for(SingalModFile & smf : mh.data){
            applyConfigToPython(smf.config,"mf");
            if(!smf.mfs.OnInit){
                al("Cannot find OnInit in mod \"" + smf.config.modName + "\" Path:" + smf.loadFileName + "!So,we won't call this function.\n");
                continue;
            }
            PyObject * rt = PyObject_CallObject(smf.mfs.OnInit,NULL);
            if(rt == Py_False){
                al("This mod \"" + smf.config.modName + "\" path \"" + smf.loadFileName +"\"'s OnInit function returns False in python.It loaded fail!");
                cout << "load mod \"" << smf.loadFileName << "\" fail!" << endl;
                continue;
            }
        }
        al("Getting initialized values...");
        ///Getting inited values
        showingStrings = getStringLists(PyObject_GetAttrString(GetPm().mlcPointer,MENU_STRINGS_ITEM));
        if(showingStrings.size() == 0){
            al("One mod may set showStrings to empty and maybe locked changing this item!We will add an empty value to prevent crash!");
            ///Prevent to crash
            showingStrings.push_back("");
        }
    }

    ///Last Step:Checkings
    if(modLoadingGood && doLoadMtMod){
        ///Checking Logs
        al("Checking mods' logs...");
        string modlog = PyObjectToString(GetPm().log);
        ssep;
        al("Here are the mod logs that are listed:");
        ls << (modlog + "\n");
        ssep;
    }
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
void LogSaver::operator <<(string v){if(openedStoring)m_buffer += v;}
void LogSaver::operator <<(int v){(*this) << to_string(v);}
void LogSaver::operator <<(double v){(*this) << to_string(v);}
void LogSaver::operator <<(float v){(*this) << to_string(v);}
void LogSaver::operator <<(char * v){(*this) << string(v);}
