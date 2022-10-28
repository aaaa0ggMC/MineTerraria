#include "mods.h"

using namespace std;
using namespace sf;
using namespace rapidjson;

SingalModData::SingalModData(string d,string p){
    data = d;
    path = p;
}

ModsHelper::~ModsHelper(){
    this->data.clear();
}

void ModsHelper::append(ModConfig & mcg,vector<SingalModData> & dta){
    data.push_back(SingalModFile(mcg,dta));
}

int readModConfig(string manifestFile,ModConfig & config){
    Document doc;
    doc.Parse(manifestFile.c_str());
    if(doc.HasParseError())return ER_JSON_PARSE;
    Value & v = doc;
    //Check Pack
    char * ns = (char *)MOD_PACK_NAME;
    if(v.HasMember(ns) && v[ns].IsString()){
        config.modName = string(v[ns].GetString());
    }else{
        config.modName = UNKNOWN_MOD_NAME;
    }
    //Check Author
    ns = (char *)MOD_AUTHOR_NAME;
    if(v.HasMember(ns)){
        if(v[ns].IsString()){
            config.modAuthor = string(v[ns].GetString());
        }
    }else{
        config.modAuthor = UNKNOWN_MOD_NAME;
    }
    //Check Mod Ver
    ns = (char *)MOD_VERSION_NAME;
    if(v.HasMember(ns)){
        if(v[ns].IsString()){
            config.modVersion = string(v[ns].GetString());
        }
    }else{
        config.modVersion = UNKNOWN_MOD_NAME;
    }
    //Check Des
    ns = (char *)MOD_DESCTIPTION_NAME;
    if(v.HasMember(ns)){
        if(v[ns].IsString()){
            config.description = string(v[ns].GetString());
        }
    }else{
        config.description = UNKNOWN_MOD_NAME;
    }
    //Check Mod ID
    ns = (char *)MOD_ID_NAME;
    if(v.HasMember(ns)){
        if(v[ns].IsString()){
            config.modPack = string(v[ns].GetString());
        }
    }else{
        config.modPack = UNKNOWN_MOD_NAME;
    }
    //Check Entry
    ns = (char *)MOD_ENTRY_NAME;
    if(v.HasMember(ns)){
        if(v[ns].IsString()){
            config.script_main = string(v[ns].GetString());
        }
    }else{
        config.script_main = UNKNOWN_MOD_NAME;
    }
    //Check Root
    ns = (char *)MOD_ROOT_NAME;
    if(v.HasMember(ns)){
        if(v[ns].IsString()){
            config.root = string(v[ns].GetString());
        }
    }else{
        if(!config.script_main.compare(UNKNOWN_MOD_NAME)){
            ///TODO:fix this return
            return EXECUTE_FAI;
        }
        config.root = UNKNOWN_MOD_NAME;
    }
    return EXECUTE_SUC;
}

void SetManifestGetFunc(ModConfig & mcg){
    PyRun_SimpleString("print('Inited before:'," MANIFEST_GET_FUNC ")");
    string runS = "def mdef_" MANIFEST_GET_FUNC "(wt):\n    if(wt == 'ModName'):\n        return '";
    runS += mcg.modName;
    runS += "'\n    if(wt == 'ModVersion'):\n        return '";
    runS += mcg.modVersion;
    runS += "'\n";
    runS += "\n" MANIFEST_GET_FUNC " = mdef_" MANIFEST_GET_FUNC "\n";
    PyRun_SimpleString(runS.c_str());
    PyRun_SimpleString("print('Inited Attr:'," MANIFEST_GET_FUNC ")");
}

int ModSetCheckMod(ModSet & modset,string path){
    for(SingalModFile & smf : modset){
        if(!smf.config.modName.compare(path)){
            return 1;
        }
    }
    return 0;
}

#define ss(x) string(x)
#define AddPyEleEx(adder,vname,attr,value,endTok) adder += ss(vname) + ss(".") + ss(attr) + ss(" = ") + ss("\"") + ss(value) + ss("\"") + ss(endTok)
#define AddPyEle(adder,vname,attr,value) AddPyEleEx(adder,vname,attr,value,"\n")

void applyConfigToPython(ModConfig & mcg,string vn){
    //Now Supports :
    /*
    ModName = "" MOD_PACK
    ModVersion = "" MOD_VERSION
    ModAuthor = "" MOD_AUTHOR
    ModDescription = "" MOD_DES
    ModPackage = "" MOD_ID
    */
    string runS = "from mtmod_loader import *\n";
    AddPyEle(runS,vn,MOD_PACK_NAME,mcg.modName);
    AddPyEle(runS,vn,MOD_VERSION_NAME,mcg.modVersion);
    AddPyEle(runS,vn,MOD_AUTHOR_NAME,mcg.modAuthor);
    AddPyEle(runS,vn,MOD_DESCTIPTION_NAME,mcg.description);
    AddPyEle(runS,vn,MOD_ID_NAME,mcg.modPack);
    outs(runS);
    PyRun_SimpleString(runS.c_str());
}

int ModDataCheckData(ModData & moddata,string path){
    for(SingalModData & smd : moddata){
        if(!smd.path.compare(path)){
            return 1;
        }
    }
    return 0;
}

SingalModData * ModDataFindData(ModData & moddata,string path){
    for(SingalModData & smd : moddata){
        if(!smd.path.compare(path)){
            return &smd;
        }
    }
    return NULL;
}
