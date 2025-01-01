#pragma once
#include "uiBasics.h"
#include "uiElement.h"
#include "uiRoot.h"


/*
A UI-Page is like a uiGroup but it covers the whole screen and only draws it child elements 
*/

class uiRoot;

class uiPage: public uiElement{
    protected:
        uiRoot* root;
        //void selectFocusReceiverMethod(uiElement* receiver);
    public:    
        uiPage(); //constructor
        uiPage(uiElement* _child); //constructor
        ~uiPage(); //destructor
        void setRoot(uiRoot* _root);
        //void receiveFocus(uiElement* sender);
        void receiveFocus(uiRoot* sender);
        void receiveFocus(uiElement* sender) override;
        uiClassHirachyType getUIClassHirachyType();
        //void react(UserAction ua);
};



