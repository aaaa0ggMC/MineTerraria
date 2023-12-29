#include "requests.h"
#include <stack>
#include <iostream>

using namespace game;
using namespace std;
using namespace alib;

RequestList::RequestList(){
    InitializeCriticalSection(&cs);
}

RequestList::~RequestList(){
    DeleteCriticalSection(&cs);
}

bool RequestList::empty(){
    CriticalLock lock(cs);
    return requests.empty();
}

Request RequestList::push(Request r){
    CriticalLock lock(cs);
    requests.push(r);
    return r;
}

Request RequestList::pop(){
    CriticalLock lock(cs);
    if(requests.size()<= 0)return Request(Request::NoRequest);
    Request r = requests.top();
    requests.pop();
    return r;
}
