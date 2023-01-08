#include "requests.h"
#include <stack>
#include <iostream>

using namespace game;
using namespace std;

#define SPIN_TIME 5

Request RequestList::push(Request r){
    while(m_locking)Sleep(SPIN_TIME);
    SimpleLock sp(&m_locking);
    requests.push(r);
    return r;
}

Request RequestList::pop(){
    while(m_locking)Sleep(SPIN_TIME);
    SimpleLock sp(&m_locking);
    if(requests.size()<= 0)return Request(Request::NoRequest);
    Request r = requests.top();
    requests.pop();
    return r;
}
