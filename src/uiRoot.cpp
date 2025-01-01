#include "uiRoot.h"

uiRoot::uiRoot(){

};

uiRoot::uiRoot(DisplayConfig _config){
    config = _config;
    init();
};

uiRoot::~uiRoot(){

}

void uiRoot::addPage(uiPage* page){
    Slog("addPage");
    pages.push_back(page);
    page->setRoot(this);

    /*
    if(pages.size() == 1){
        uiPage* e = pages.front();

        if(e == nullptr){
            Slog("nullptr");
        }
        e->receiveFocus(this);
    }
    */
    
}

void uiRoot::display(){ 
    if(screenOnTime==0){
        screenOnTime=millis()+ config.screenSleepTime;
    }
    energyManager(); //takes care of the sleep

    frameInfo fi;
    fi = lastFrameInfo;
    
    //clear the frame buffer to prepare for redrawing
    fi.display->clearBuffer();

    /*
    Put debug code here:
    */
    //lastFrameInfo.display->drawFrame(10,10,20,20);

    //enters the full ui drawing cycle
    FlushDisplay(&fi); 
}

bool uiRoot::goToPage(uiPage* page){
    //todo
}

bool uiRoot::goToElement(uiElement* element){
    //todo
}

void uiRoot::react(UserAction UA){
    Slog("react");
    //reset display time
    screenOnTime=millis()+ config.screenSleepTime;
    if(globalDisplayState == ScreenState::off){
        screenSwitch(ScreenState::on);
    }else{
        if(this->focus == FocusState::current){
            //the root has focus so we switch/enter a page
            switch (UA){
                case UserAction::leftButton:
                    Slog("left");
                    if(currentPage == 0){
                        currentPage = pages.size()-1;
                    }else{
                        currentPage--;
                    }
                    break;
                case UserAction::rightButton:
                    Slog("right");
                    if(currentPage == pages.size()-1){
                        currentPage = 0;
                    }else{
                        currentPage++;
                    }
                    break;
                case UserAction::enterButton:
                    //we enter the page. This means the page receives focus and we switch to child
                    Slog("enter");
                    //remove selection from old child //this is wrong. At this point only root should have focus
                    //pages.at(currentPage)->removeFocus(this); //should it?
                    //Slog("removeFocus");
    

                    focus = FocusState::child;

                    Slog("push focus to");
                    pages.at(currentPage)->receiveFocus(this);
                    break;
                default:
                    //do nothing!

                    //here a callback should be called
                    Slog("unknown action");
                    break;
            }
        }
        else{
            //the child should react
            pages.at(currentPage)->react(UA);
        }
    }
}

void uiRoot::setConfig(DisplayConfig _config){
    config = _config;
    init();
}

void uiRoot::init(){
    config.display->begin();
    config.display->setFont(u8g2_font_6x10_tf);
    config.display->setFontRefHeightExtendedText();
    config.display->setDrawColor(1);
    config.display->setFontPosCenter();
    config.display->setFontDirection(0);

    config.display->sendBuffer();

    lastFrameInfo.display = config.display;
}

void uiRoot::FlushDisplay(frameInfo* fi){
  if(nextDisplayFrame<millis()){
    nextDisplayFrame=millis()+config.frameDistance;
    if(globalDisplayState==ScreenState::on){

        if(millis()-lastDisplayFlash >= config.flashHalfCycle){
            lastDisplayFlash = millis();
            fi->highlightSelected = !lastFrameInfo.highlightSelected;
        }  

        lastFrameInfo = *fi;

        drawUI(fi);
        config.display->sendBuffer();
    }
  }
}

void uiRoot::drawUI(frameInfo* fi){
    pages[currentPage]->draw(fi);
}

ScreenState uiRoot::getScreenState(){
    return globalDisplayState;
}

void uiRoot::wakeUpScreen(){
    screenSwitch(ScreenState::on);
    lastInputTime = millis();
}

void uiRoot::screenSwitch(ScreenState state){
  //switches Screen on or off
  if(state == ScreenState::off){
    //switch off
    if(globalDisplayState==ScreenState::on){
      config.display->clearBuffer();
      config.display->sendBuffer();
      globalDisplayState=ScreenState::off;
    }
  }
  else{
    //switch on
    if(globalDisplayState==ScreenState::off){
      globalDisplayState=ScreenState::on;
      FlushDisplay(&lastFrameInfo);
    }
  }
}

void uiRoot::energyManager(){
    //keeps track of time and manages ScreenTime
  
    //check display time
    if(screenOnTime<=millis() && config.useSleep){
        //turn off
        //Slog("enter screen sleep")
        screenSwitch(ScreenState::off);
    }
}

void uiRoot::receiveFocus(){
    if(pages.size()>1){
        //Slog("got focus")
        focus = FocusState::current;
    }else{
        //reject focus
        //Slog("would like to reject focus")
        focus = FocusState::current;
        //focus = FocusState::child;
        //pages.at(currentPage)->receiveFocus(this);
    }
   
}

void uiRoot::showStartupScreen(){
    config.display->drawFrame(config.viewportOffset.dimension.getX(),config.viewportOffset.dimension.getY(),config.viewportOffset.dimension.getWidth(),config.viewportOffset.dimension.getHeight());
    config.display->drawStr(23,10,"Initilizing UI");
    config.display->drawStr(12,25,"made with SimpleUI");
    config.display->drawStr(50,40, SIMPLEUI_VERSION_STRING);
    config.display->drawStr(16,55,"by Elias_Technik");
    config.display->sendBuffer();
}