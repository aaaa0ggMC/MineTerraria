#ifndef REQUESTS_H_INCLUDED
#define REQUESTS_H_INCLUDED
#include <stack>
#include <windows.h>
#include "../TerrarianNew.h"

using namespace std;

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

    struct SimpleLock{
        bool * b;
        SimpleLock(bool * b){
            this->b = b;
            *b = true;
        }
        ~SimpleLock(){
            *b = false;
        }
    };

    struct RequestList{
        Request push(Request);
        Request pop();
        Request forcePop();
        bool empty(){return requests.empty();}
        bool m_locking;
    private:
        stack<Request> requests;
    public:
        RequestList(){
            m_locking = false;
        }
    };
}

#endif // REQUESTS_H_INCLUDED
