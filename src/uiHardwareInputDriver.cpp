#include "uiHardwareInputDriver.h"

//HardwareButton

uiHardwareButton::uiHardwareButton(){
    action = UserAction::none;
    floatingValue = true;
    lastReadValue = floatingValue;
    state = ButtonStates::up;
    autopressDelay = DEFAULT_AUTOPRESS_DELAY;
    triggerAutopressAfter = 0;
    triggerAutopressActionAfter = 0;
    requestSatisfied = true;
    enableLongpress = false;
    enableAutopress = false;
}

uiHardwareButton::uiHardwareButton(uint8_t _pin, UserAction _action, bool _expectedFloatingValue){
    pin = _pin;
    action = _action;
    floatingValue = _expectedFloatingValue;
    lastReadValue = floatingValue;
    state = ButtonStates::up;
    triggerAutopressAfter = 0;
    triggerAutopressActionAfter = 0;
    requestSatisfied = true;
    enableLongpress = false;
    enableAutopress = false;
}

void uiHardwareButton::configureAutopress(unsigned long _autopressSpeed, unsigned long _autopressDelay){
    autopressSpeed = _autopressSpeed;
    autopressDelay = _autopressDelay;
};

void uiHardwareButton::configureLongpress(UserAction _secondaryAction, unsigned long _longpressDelay){
    longpressDelay = _longpressDelay;
    secondAction = _secondaryAction;
};

void uiHardwareButton::switchAutopress(bool _enableAutopress){
    enableAutopress = _enableAutopress;
    if(enableLongpress){
        enableLongpress = false;
    }
};

void uiHardwareButton::switchLongpress(bool _enableLongpress){
    enableLongpress = _enableLongpress;
    if(enableAutopress){
        enableAutopress = false;
    }  
};

void uiHardwareButton::trigger(){
    //check previous state
    bool v=digitalRead(pin);
    if(lastReadValue==!floatingValue && v==floatingValue){
        //released
        state=ButtonStates::up;
        if(blockLongPress){
            //the last press was a long press and it was satisfied. This releas should not trigger a short press
            blockLongPress = false;
        }else{
            //shortpress
            if(millis()>unlockAfter){
                request=ButtonRequest::shortpress;
                requestSatisfied=false;
            }else{
                UI_INFO("blocked shortpress because the last satisfy was just a moment ago [DEFAULT_LOCK_TIME]","uiHardwareButton");
            }
        }
    }
    if(lastReadValue==floatingValue && v==!floatingValue){
        //pressed
        state=ButtonStates::down;
        triggerAutopressAfter=millis()+autopressDelay;
        triggerLongPressAfter=millis()+longpressDelay;
    }
    lastReadValue=v;
}

ButtonStates uiHardwareButton::getState(){
    return state;
}

UserAction uiHardwareButton::getAction(){
    return action;
}

UserAction uiHardwareButton::getLongpressAction(){
    return secondAction;
}

bool uiHardwareButton::isAutopressing(){
    if(enableAutopress && !enableLongpress){
        if(triggerAutopressAfter<millis() && state == ButtonStates::down){
            request=ButtonRequest::autopress;
            requestSatisfied=false; //?
            return true;
        }
        else{
            return false;
        }
    }else{
        return false;
    }
}

bool uiHardwareButton::isLongpressing(){
    if(enableLongpress && !enableAutopress){
        if(triggerLongPressAfter<millis() && state == ButtonStates::down){
            if(!blockLongPress && millis()>unlockAfter){
                request=ButtonRequest::longpress;
                requestSatisfied=false;
                return true;
            }else{
                return false;
            }
        }
        else{
            return false;
        }
    }else{
        return false;
    }
}

void uiHardwareButton::satisfyAutopressRequest(){
    triggerAutopressActionAfter = millis()+autopressSpeed;
}

bool uiHardwareButton::hasPendingRequests(){
    if(state == ButtonStates::down){
        return isLongpressing() || isAutopressing();
    }else{
        return !requestSatisfied && request == ButtonRequest::shortpress;
    }
}

void uiHardwareButton::satisfyRequest(){
    unlockAfter = millis()+DEFAULT_LOCK_TIME;
    requestSatisfied = true;
    request = ButtonRequest::none;
}

void uiHardwareButton::satisfyLongpressRequest(){
    blockLongPress = true;
    unlockAfter = millis()+DEFAULT_LOCK_TIME;
    requestSatisfied = true;
    request = ButtonRequest::none;
}


//uiHardwareInputDriver
uiHardwareInputDriver::uiHardwareInputDriver(uiRoot* _uiRoot){
    root = _uiRoot;
}

void uiHardwareInputDriver::addInput(uiHardwareButton* input){
    inputs.push_back(input);
}

void uiHardwareInputDriver::inputInsertCallback(UserAction UA){
    if(inputInsertMode == uiHIDInputInsertCBMode::notifyAndReact){
        SafeCallback(inputInsert,inputInsert(UA));
        root->react(UA);
    }else if(inputInsertMode == uiHIDInputInsertCBMode::ReactOnly){
        root->react(UA);
    }else if(inputInsertMode == uiHIDInputInsertCBMode::notifyOnly){
        SafeCallback(inputInsert,inputInsert(UA));
    }
}

void uiHardwareInputDriver::injectHardwareInput(UserAction UA){
    inputInsertCallback(UA);
}

void uiHardwareInputDriver::run(){
    for(uiHardwareButton* input : inputs){
        if(input->hasPendingRequests()){
            if(input->isLongpressing()){
                //longpress
                if(unlockLongpressAfter<millis()){
                    unlockLongpressAfter = 0;
                    inputInsertCallback(input->getLongpressAction());
                    SafeCallback(onReact,onReact(nullptr));
                    input->satisfyLongpressRequest();
                    UI_DEBUG("longpress","HID");
                }
            }else{
                //shortpress
                inputInsertCallback(input->getAction());
                SafeCallback(onReact,onReact(nullptr));
                input->satisfyRequest();
                UI_DEBUG("shortpress","HID");
            }
            
        }
        if(input->isAutopressing()){
            root->react(input->getAction());
            SafeCallback(onReact,onReact(nullptr));
            input->satisfyAutopressRequest();
            UI_DEBUG("autopress","HID");
        }
    }
}

void uiHardwareInputDriver::injectInput(UserAction action){
    root->react(action);
}



void uiHardwareInputDriver::setOnReactCallback(uiNotifyCallback callback){
    onReact = callback;
}

void uiHardwareInputDriver::setInputInsertCallback(uiInputCallback callback, uiHIDInputInsertCBMode mode){
    inputInsert = callback;
    inputInsertMode = mode;
}

void uiHardwareInputDriver::switchInputInsertCallbackMode(uiHIDInputInsertCBMode mode){
    inputInsertMode = mode;
}

void uiHardwareInputDriver::blockLongpressUntilLifted(){
    UI_DEBUG("Blocking any longpress until lifted","HID")
    for(uiHardwareButton* input : inputs){
        if(input->isLongpressing()){
            UI_DEBUG("Longpress blocked.", "HID")
            input->satisfyLongpressRequest();
        }
    }
}

void uiHardwareInputDriver::timeoutLongpress(unsigned long timeout){
    unlockLongpressAfter = millis()+timeout;
    UI_DEBUG("Longpress timeout set","HID")
}