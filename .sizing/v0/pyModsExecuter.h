#ifndef PYMODSEXECUTER_H_INCLUDED
#define PYMODSEXECUTER_H_INCLUDED
#include "kernel.h"
#include "cppPy/CppPy.h"

using namespace std;

#define ERR_PY_NOT_INIT    101
#define ERR_INIT_ROOT_FAIL 102
#define ERR_GET_KERNEL_VAR 103
#define ERR_GET_MENU_ITEMS 104
#define ERR_LOAD_KERNEL 105
#define ERR_FAIL_COMPILE 106
#define ERR_INIT_MOD 107
#define ERR_GET_FUNCTION 108

#define EX_ROOT "res/api/mtmod_loader.py"

#define SC_ROOT "mtmod_loader.py"
#define MD_ROOT "mtmod_loader"

#define MENU_STRINGS_ITEM "menuStrings"
#define LOADING_PROC_ITEM "nowProc"
#define LOADING_LOG_ITEM  "modLog"

struct PyObject_ModModified{
    PyObject * mlcPointer;
    PyObject * menuStringsPointer;
    PyObject * processId;//0:initializing
    PyObject * log;
    PyObject * kernel;
};

struct ModFile_Struct{
    PyObject * OnInit;
};

struct ModModifiedItems{
    vector<string> menuStrings;
};

typedef vector<ModModifiedItems> ModMdSet;

int readModData(string modMainFile,string modFileModuleName,ModFile_Struct & mfs);
int initModRootFile(string rootFile);
void setFuc();

void SetPm(PyObject_ModModified & pmd);
PyObject_ModModified GetPm();

#endif // PYMODSEXECUTER_H_INCLUDED
