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
    UI_DEBUG("addPage","uiRoot");
    pages.push_back(page);
    page->setRoot(this);
}

bool uiRoot::isScreenOn(){
    return globalDisplayState == ScreenState::on;
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

void uiRoot::goToPage(uiPage* page){
    UI_DEBUG("goToPage","uiRoot");
    //find the page
    UI_DEBUG("find page","uiRoot");
    if(page != nullptr){
        if(getPageID(page) != -1){
            //reset all focus
            for(uiPage* page : pages){
                page->resetFocusAndSelection(true);
            }
            //switch to the page
            currentPage = getPageID(page);
            focus = FocusState::child;
            pages.at(currentPage)->receiveFocus(this, true);
        }else{
            UI_ERROR("err: Page not found","uiRoot");  
        }  
    }else{  
        UI_ERROR("err: Page is null","uiRoot");
    } 
}

void uiRoot::goToElement(uiElement* element){
    shiftFocusTo(element);
}

void uiRoot::react(UserAction UA){
    UI_DEBUG("react","uiRoot");
    //reset display time
    screenOnTime=millis()+ config.screenSleepTime;
    if(globalDisplayState == ScreenState::off){
        screenSwitch(ScreenState::on);
    }else{
        if(this->focus == FocusState::current){
            if(!isPageNavigationDeactivated){
                //the root has focus so we switch/enter a page
                switch (UA){
                    case UserAction::leftButton:
                        UI_DEBUG("left","uiRoot");
                        if(currentPage == 0){
                            currentPage = pages.size()-1;
                        }else{
                            currentPage--;
                        }
                        break;
                    case UserAction::rightButton:
                        UI_DEBUG("right","uiRoot");
                        if(currentPage == pages.size()-1){
                            currentPage = 0;
                        }else{
                            currentPage++;
                        }
                        break;
                    case UserAction::enterButton:
                        //we enter the page. This means the page receives focus and we switch to child
                        UI_DEBUG("enter","uiRoot");
                        focus = FocusState::child;

                        UI_DEBUG("push focus to","uiRoot");
                        pages.at(currentPage)->receiveFocus(this, false);
                        break;
                    default:
                        //do nothing!

                        //here a callback should be called
                        UI_DEBUG("unknown action","uiRoot");
                        break;
                }
            }else{
                UI_INFO("Page navigation deactivated.","uiRoot");
            }
            
        }
        else{
            //the child should react
            pages.at(currentPage)->react(UA);
        }
    }
}

int uiRoot::getPageID(uiPage * page){
    auto it = find(pages.begin(), pages.end(), page);

    // If element was found
    if (it != pages.end()){
        // calculating the index
        return it - pages.begin();
    }
    else {
        // If the element is not present in the vector
        return -1;
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
    screenOnTime=millis()+ config.screenSleepTime;
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
    if(isPageNavigationDeactivated){
        //bounce focus back to page
        pages.at(currentPage)->receiveFocus(this, true);
    }else{
        focus = FocusState::current;
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

void uiRoot::shiftFocusTo(uiElement* e){
    //reset all focus
    for(uiPage* page : pages){
        page->resetFocusAndSelection(true);
    }

    //find the element
    UI_DEBUG("find element","uiRoot");
    for(uiPage* page : pages){
        if(page->isInChildBranch(e)){
            page->shiftFocusTo(e);
            currentPage = getPageID(page);
            focus = FocusState::child;
            return;
        }
    }
    UI_ERROR("err: Element not found","uiRoot");                                           
}

int uiRoot::getCurrentPageID(){
    return currentPage;
}

uiPage* uiRoot::getCurrentPage(){
    return pages.at(currentPage);
}

void uiRoot::removePage(uiPage* page){
    auto it = find(pages.begin(), pages.end(), page);
    if (it != pages.end()){
        pages.erase(it);
    }
}

void uiRoot::printTree(HardwareSerial * s){
    s->println("UI-Tree:");
    if(focus == FocusState::current){
        s->println("[uiRoot]");
    }else{
        s->println("uiRoot");
    }
    for(uiPage* page : pages){
        if(getPageID(page) == pages.size()-1){
            page->printTree(s, "     ", "└── ");
        }else{
            page->printTree(s, "│    ", "├── ");
        }
    }
}

uiPage* uiRoot::getPageByID(String id){
    for(uiPage* page : pages){
        if(page->getID() == id){
            return page;
        }
    }
    return nullptr;
}

void uiRoot::deactivatePageNavigation(bool deactivate){
    UI_DEBUG("deactivatePageNavigation","uiRoot");
    isPageNavigationDeactivated = deactivate;
    if(pages.size()>currentPage){
        uiPage* cp = pages.at(currentPage);
        if(cp != nullptr){
            cp->receiveFocus(this, true);
        }
    }
    
}