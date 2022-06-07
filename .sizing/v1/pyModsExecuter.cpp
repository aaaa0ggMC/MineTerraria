#include "pyModsExecuter.h"

using namespace std;

static PyObject_ModModified pm;

int readModData(string modMainFile,string modFileModuleName,ModFile_Struct & mfs){
    if(!Py_IsInitialized()){
        return ERR_PY_NOT_INIT;
    }
    PyObject * pcode = Py_CompileString(modMainFile.c_str(),modFileModuleName.c_str(),Py_file_input);
    if(pcode){
        PyObject * md = PyImport_ExecCodeModule(modFileModuleName.c_str(),pcode);
        if(md){
            PyObject * oninitFunc = PyObject_GetAttrString(md,"OnInit");
            if(oninitFunc){
                mfs.OnInit = oninitFunc;
            }else{
                //Added a new rule,we do not throw exceptions,while we set it to null
                mfs.OnInit = NULL;
            }
        }else{
            return ERR_GET_KERNEL_VAR;
        }
    }else{
        return ERR_INIT_MOD;
    }
    return EXECUTE_SUC;
}

void SetPm(PyObject_ModModified & pmd){
    pm = pmd;
}

PyObject_ModModified GetPm(){
    return pm;
}

int initModRootFile(string rootFile){
    PyObject * pcode = Py_CompileString(rootFile.c_str(),SC_ROOT,Py_file_input);
    if(pcode){
        PyObject * md = PyImport_ExecCodeModule(MD_ROOT,pcode);
        pm.kernel = md;
        PyObject * mlc = PyObject_GetAttrString(md,"mlc");
        if(!mlc){
            return ERR_GET_KERNEL_VAR;
        }
        pm.mlcPointer = mlc;
        ///Guessed Mistake:list are changing,get the address is useless
        ///@ThisIsDeprecated
        PyObject * menuString = PyObject_GetAttrString(mlc,MENU_STRINGS_ITEM);
        if(!menuString){
            return  ERR_GET_MENU_ITEMS;
        }
        pm.menuStringsPointer = menuString;
        //Clear Flags
        PyRun_SimpleString("from mtmod_loader import *\nmlc." MENU_STRINGS_ITEM ".clear()");

        menuString = PyObject_GetAttrString(mlc,LOADING_PROC_ITEM);
        if(!menuString){
            return  ERR_GET_MENU_ITEMS;
        }

        pm.processId = menuString;
        menuString = PyObject_GetAttrString(mlc,LOADING_LOG_ITEM);
        if(!menuString){
            return  ERR_GET_MENU_ITEMS;
        }
        pm.log = menuString;
    }else{
        return ERR_INIT_ROOT_FAIL;
    }
    return EXECUTE_SUC;
}
