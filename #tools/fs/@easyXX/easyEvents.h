#ifndef EASYEVENTS_H_INCLUDED
#define EASYEVENTS_H_INCLUDED

struct HelperEvents{
    int mouseRel;
    int mousePre;
    int mouseMov;
    int keyPre;
    int keyRel;

    HelperEvents();
    void Origin();
};

#endif // EASYEVENTS_H_INCLUDED
