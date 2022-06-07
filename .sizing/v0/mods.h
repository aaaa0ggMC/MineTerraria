#ifndef MODS_H_INCLUDED
#define MODS_H_INCLUDED
#include "kernel.h"
#include "pyModsExecuter.h"


///Notes
#define NOTE "\n[note]:"
#define NT_CANNNOT_ALLOC NOTE "Oh gosh!Your computer's memory seemed to be full!Are you running Minecraft Java Edition?Or you put me into a sandbox and restricts my usable memories."
#define NT_BAD_MOD       NOTE "Thinking about somebody tricks you and gives you a bad mod file (:D)!"
#define NT_SAME_MOD       NOTE "Delete one of the mod can help!!"

///Mods Configs

#define MOD_UI_START_X 100
#define MOD_UI_START_Y 100
#define MOD_UI_SPLIT_X 64
#define MOD_UI_SPLIT_Y 40

#define MOD_MANIFEST_PATH "main.manifest"
#define MOD_PACK_NAME "ModName"
#define MOD_AUTHOR_NAME "ModAuthor"
#define MOD_VERSION_NAME "ModVersion"
#define MOD_DESCTIPTION_NAME "ModDescription"
#define MOD_ID_NAME "ModPackage"
#define MOD_ENTRY_NAME "ModEntry"
#define MOD_ROOT_NAME "ModRoot"
#define MOD_SCRIPT_PATH_NAME "ScriptPath"
//·ÀÖ¹Ä£×éÃüÃû³åÍ»
#define UNKNOWN_MOD_NAME "_djjf**E!8841__U*((!1n98218k**!*no*@!*#wn_"

#define MANIFEST_GET_FUNC "getAttr"

#define LockPyFunctionsStr "def OnInit():\n    print(mf.ModName,\"doesn't has function OnInit,pass the initializing process!\")"
#define LockPyFunctions    PyRun_SimpleString(LockPyFunctionsStr)

using namespace sf;
using namespace std;
using namespace rapidjson;

///Mod Script Entry:Fixed:./script/main.[mod_file_type]
struct ModConfig{
    string modName;
    string modPack;///This is the mod's id!
    string modVersion;
    string modAuthor;
    string description;
    //string script_path;
    string root;
    string script_main;
    ModConfig(string mname = UNKNOWN_MOD_NAME,string modVer = UNKNOWN_MOD_NAME,
              string modA = UNKNOWN_MOD_NAME,string dst = UNKNOWN_MOD_NAME,
              string modp = UNKNOWN_MOD_NAME,string reserved0 = UNKNOWN_MOD_NAME,string rt = UNKNOWN_MOD_NAME,string ent = UNKNOWN_MOD_NAME){
        modName = mname;
        modVersion = modVer;
        modAuthor = modA;
        modPack = modp;
        description = dst;
        //script_path = sp;
        script_main = ent;
        root  = rt;
    }
};

struct ModShow{
    Text modName;
    Text modVer;
    Text modAut;
    ModShow(Text& mn,Text & mv,Text & ma){modName = mn;modVer = mv;modAut = ma;};
    ModShow(){};
};

void applyConfigToPython(ModConfig & mcg,string vn);

int readModConfig(string manifestFile,ModConfig & config);

//typedef void(*DataFree)(SingalModData & md);
struct SingalModData{
    string path;
    string data;
    SingalModData(string dta,string path);
    SingalModData(){data="";}
};
typedef vector<SingalModData> ModData;
struct SingalModFile{
    ModConfig config;
    ModData datas;
    string loadFileName;
    ModFile_Struct mfs;
    SingalModFile(ModConfig & config,ModData & dtas,string lfn = UNKNOWN_MOD_NAME,ModFile_Struct mfs = ModFile_Struct()){
        this->config = config;
        datas = dtas;
        loadFileName = lfn;
        this->mfs = mfs;
    }
};
typedef vector<SingalModFile> ModSet;

class ModsHelper{
public:
    ModSet data;
    ModMdSet modSets;
    ~ModsHelper();
    void append(ModConfig & mcg,ModData & modData);
};

int ModSetCheckMod(ModSet & modset,string path);
int ModDataCheckData(ModData & moddata,string path);
SingalModData * ModDataFindData(ModData & moddata,string path);

void SetManifestGetFunc(ModConfig & mcg);

#endif // MODS_H_INCLUDED
