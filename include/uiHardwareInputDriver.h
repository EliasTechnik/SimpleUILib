#pragma once

#include "uiBasics.h"
#include "uiRoot.h"
#include <vector>

#define DEFAULT_AUTOPRESS_DELAY 500
#define DEFAULT_LONGPRESS_DELAY 500
#define DEFAULT_AUTOPRESS_SPEED 5
#define DEFAULT_LOCK_TIME 50

/*
The code in here handles user input from hardware devices like buttons.

*/

enum class ButtonStates{down,up};

enum ButtonRequest{none, shortpress, longpress, autopress};

class uiHardwareButton{
    private:
        uint8_t pin;
        UserAction action = UserAction::none;
        UserAction secondAction = UserAction::none;
        unsigned long triggerAutopressAfter = 0;
        unsigned long triggerAutopressActionAfter = 0;
        unsigned long triggerLongPressAfter = 0;
        bool enableLongpress = false;
        bool enableAutopress = false;
        unsigned long autopressDelay = DEFAULT_AUTOPRESS_DELAY;
        unsigned long longpressDelay = DEFAULT_LONGPRESS_DELAY;
        unsigned long autopressSpeed = DEFAULT_AUTOPRESS_SPEED;
        unsigned long unlockAfter = 0;
        bool lastReadValue = true; 
        bool floatingValue; //the expected value when the button is not pressed
        ButtonStates state = ButtonStates::up;
        bool requestSatisfied = false;
        bool blockLongPress = false;
        ButtonRequest request = ButtonRequest::none;
    public:
        uiHardwareButton();
        uiHardwareButton(uint8_t _pin, UserAction _action, bool _expectedFloatingValue = true);
        void trigger(); //this function should be called in an ISR it updates the values of the HardwarButton Object but does not run long. 
        ButtonStates getState();
        UserAction getAction();
        UserAction getLongpressAction();
        bool hasPendingRequests(); //false if there are pending requests
        bool isAutopressing(); //true if the button is in autopress mode and the user is holding it down
        bool isLongpressing(); //true if the button is in longpress mode and the user reached the longpress time
        void satisfyLongpressRequest();
        void satisfyRequest();
        void satisfyAutopressRequest();
        void switchAutopress(bool _enableAutopress);
        void switchLongpress(bool _enableLongpress);
        void configureAutopress(unsigned long _autopressSpeed = DEFAULT_AUTOPRESS_SPEED, unsigned long _autopressDelay = DEFAULT_LONGPRESS_DELAY);
        void configureLongpress(UserAction _secondaryAction = UserAction::none, unsigned long _longpressDelay = DEFAULT_LONGPRESS_DELAY);
        
};

enum uiHIDInputInsertCBMode{
    notifyAndReact, //the insertCallback is called and the UI reacts
    ReactOnly,      //the insertCallback is not called but the UI reacts
    notifyOnly,     //the insertCallback is called but the UI does not react
};

class uiHardwareInputDriver{
    protected:
        std::vector<uiHardwareButton*> inputs;
        uiRoot* root;
        uiNotifyCallback onReact = nullptr; //gets called every time the an react is triggered
        uiInputCallback inputInsert = nullptr;
        uiHIDInputInsertCBMode inputInsertMode = uiHIDInputInsertCBMode::notifyAndReact;
        void inputInsertCallback(UserAction UA);
    public:
        uiHardwareInputDriver(uiRoot* _uiRoot);
        void addInput(uiHardwareButton* input);
        void injectInput(UserAction UA); //this is used to simulate inputs and can be used when implementing on screen navigation buttons. Injected inputs are always relayed to the ui and injected after the input procesing.
        void injectHardwareInput(UserAction UA); //this is used to simulate inputs and can be used when implementing custom input logic. Other then injectInput this will be injected at the begining of the input processing.
        void run(); //this has to be called frequently in order to process inputs correctly 
        void setOnReactCallback(uiNotifyCallback _onReact); //this callback is triggered every time the ui reacts. 
        //This can be usefull for debugging or for triggering other actions after a user pressed a button.
        void setInputInsertCallback(uiInputCallback _inputInsert, uiHIDInputInsertCBMode mode = uiHIDInputInsertCBMode::notifyAndReact); 
        void switchInputInsertCallbackMode(uiHIDInputInsertCBMode mode);
};