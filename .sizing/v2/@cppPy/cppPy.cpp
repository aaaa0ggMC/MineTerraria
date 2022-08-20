#include "cppPy.h"

using namespace std;

int PyLock::init(){
    Py_Initialize();
    return Py_IsInitialized()?EXECUTE_SUC:EXECUTE_FAI;
}

PyLock::~PyLock(){
    Py_Finalize();
}

char * PyObjectToCharString(PyObject * o){
    return PyBytes_AsString(PyUnicode_AsASCIIString(PyObject_Repr(o)));
}

string PyObjectToString(PyObject * o){
    return string(PyObjectToCharString(o));
}

vector<string> getStringLists(PyObject * mlist){
    vector<string> ss;
    Py_ssize_t lsz = PyList_Size(mlist);
    for(Py_ssize_t i = 0;i < lsz;++i){
        string pushIn = PyObjectToString(PyList_GetItem(mlist,i));
        pushIn[0] = ' ';
        pushIn[pushIn.length()-1] = ' ';
        strps::Trim(pushIn);
        ss.push_back(pushIn);
    }
    return ss;
}

void apStringLists(vector<string> & ss,PyObject * lst){
    PyObject * perDta;
    for(string & s : ss){
        perDta = Py_BuildValue("s",s.c_str());
        PyList_Append(lst,perDta);
    }
}
