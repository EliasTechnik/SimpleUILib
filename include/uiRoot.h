#pragma once
#include "uiBasics.h"
#include "uiPage.h"
#include "uiElement.h"
#include <vector>
#include "version.h"

/*
The uiRoot is the entry point for the ui. It defines the states at startup and handles inputs
*/

class uiPage;

class uiRoot{
    protected:
        frameInfo lastFrameInfo;
        std::vector<uiPage*> pages;
        unsigned int currentPage = 0;
        DisplayConfig config;
        unsigned long nextDisplayFrame = 0;
        unsigned long lastInputTime = 0;
        unsigned long screenOnTime = 0;
        unsigned long lastDisplayFlash = 0;
        ScreenState globalDisplayState = ScreenState::on;
        FocusState focus = FocusState::current;
        void init();
        void FlushDisplay(frameInfo* fi);
        void drawUI(frameInfo* fi);
        void screenSwitch(ScreenState state);
        void energyManager();
        int getPageID(uiPage* page);
    public:
        uiRoot();
        uiRoot(DisplayConfig _config);
        ~uiRoot();
        void addPage(uiPage* page);
        void goToPage(uiPage* page);
        void goToElement(uiElement* element);
        void display(); //should be called every time in the loop()
        void react(UserAction UA);
        void setConfig(DisplayConfig _config);
        ScreenState getScreenState();
        void wakeUpScreen();
        bool isScreenOn();
        void receiveFocus();
        void showStartupScreen();
        void shiftFocusTo(uiElement* e);
        int getCurrentPageID();
        uiPage* getCurrentPage();
        void removePage(uiPage* page);
        void printTree(HardwareSerial * s); //prints the ui tree to the given serial
        //uiClassHirachyType getUIClassHirachyType() override;
};

