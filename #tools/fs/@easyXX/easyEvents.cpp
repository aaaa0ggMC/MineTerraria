#include "easyEvents.h"

void HelperEvents::Origin(){
    this->mouseMov = this->mousePre = this->mouseRel = this->keyRel = this->keyPre = -1;
}

HelperEvents::HelperEvents(){
    Origin();
}
