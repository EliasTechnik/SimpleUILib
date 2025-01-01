#pragma once
#include "uiBasics.h"
#include "uiElement.h"

/**/


/*uiInteractiveElement is the baseclass for all interactive UI-Elements*/
class uiInteractive{
    protected:
        tabPos tabPosition;
        bool focus; //true if element has focus (display inverted)
        uiCallback CBonEvent; 
    public:
        uiInteractive();
        void setCB(uiCallback _CBonEvent);
        void triggerUIaction(UIEventType triggerType);
        void setFocus(bool _focus = true);
};

