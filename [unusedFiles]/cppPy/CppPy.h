#ifndef CPPPY_H_INCLUDED
#define CPPPY_H_INCLUDED
#include "../python/Python.h"
#include "../kernel.h"

using namespace std;

class noncopyable
{
 public:
  noncopyable(const noncopyable&) = delete;
  void operator=(const noncopyable&) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

class PyLock : public noncopyable{
public:
    int init();
    ~PyLock();
};

char * PyObjectToCharString(PyObject * o);
string PyObjectToString(PyObject * o);

vector<string> getStringLists(PyObject * mlist);
void apStringLists(vector<string> & ss,PyObject * lst);

#endif // CPPPY_H_INCLUDED
