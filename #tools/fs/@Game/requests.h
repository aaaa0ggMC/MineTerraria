#ifndef REQUESTS_H_INCLUDED
#define REQUESTS_H_INCLUDED
#include <stack>
#include <windows.h>
#include <spdlog.h>
#include "../TerrarianNew.h"

using namespace std;
using namespace alib;

#define MAKE_REQV(n,t,s,v) {Request r = Request(Request::t);r.s = v;n.push(r);}
#define MAKE_REQ(t,s,v) {Request r = Request(Request::t);r.s = v;rlist.push(r);}

namespace game{
    struct Request{
        enum QuestType{
            LoadChunk,UnloadChunk,NoRequest
        };
        QuestType qt;
        union{
            Chunk * targetChunk;
        };
        Request(QuestType qt){
            this->qt = qt;
        }
    };

    struct RequestList{
        Request push(Request);
        Request pop();
        bool empty();
        RequestList();
        ~RequestList();
    private:
        stack<Request> requests;
        CRITICAL_SECTION cs;
    };
}

#endif // REQUESTS_H_INCLUDED
