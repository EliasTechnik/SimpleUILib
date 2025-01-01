#include "uiInteractive.h"

//uiInteractive
uiInteractive::uiInteractive(){
    tabPosition.horizontalTabId = 0;
    tabPosition.verticalTabId = 0;
    CBonEvent.CBcontext = nullptr;
    CBonEvent.CB = nullptr;
};

void uiInteractive::setCB(uiCallback _CBonEvent){
    CBonEvent = _CBonEvent;
}

void uiInteractive::triggerUIaction(UIEventType triggerType){
    if(CBonEvent.CBcontext != nullptr){
        CBonEvent.CB(CBonEvent.CBcontext, this, triggerType);
    }
};

void uiInteractive::setFocus(bool _focus){
    focus = _focus;
};
