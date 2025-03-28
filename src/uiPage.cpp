#include "uiPage.h"

uiPage::uiPage(){
    selectionMode = SelectionMode::passthroughSelection;
    focusMode = FocusMode::passthrough;
    focus = FocusState::parent;
    visible = true;
    id = "uiPage";
};

uiPage::uiPage(uiElement* _child){
    addChild(_child, true);
    selectionMode = SelectionMode::passthroughSelection;
    focusMode = FocusMode::passthrough;
    focus = FocusState::parent;
    visible = true;
    id = "uiPage";
};

uiPage::~uiPage(){
};

void uiPage::setRoot(uiRoot* _root){
    root = _root;
}

void uiPage::receiveFocus(uiRoot* sender, bool isPreselection){
    focus = FocusState::child;
    UI_DEBUG("uiPage: relay to child",id);
    childWithFocus = focusChild;
    focusChild->receiveFocus(this, isPreselection);
}

void uiPage::receiveFocus(uiElement* sender, bool isPreselection){
    focus = FocusState::parent;
    UI_DEBUG("uiPage: relay to parent",id);
    root->receiveFocus(); //when relaying to root we obmit the isPreselection flag (because the root does not care)
};

uiClassHirachyType uiPage::getUIClassHirachyType(){
    return uiClassHirachyType::page;
};

void uiPage::resetFocusAndSelection(bool recursive){
    UI_DEBUG("resetFocusAndSelection",id);
    focus = FocusState::parent;
    selected = SelectionState::notSelected;
    if(recursive){
        if(childs.size()>0){
            for(uiElement* child : childs){
                child->resetFocusAndSelection(true);
            }
        }
    }
};

void uiPage::shiftFocusTo(uiElement* e){
    if(e != nullptr){
        if(isInChildBranch(e)){
            for(uiElement* child : childs){
                if(child->isInChildBranch(e)){
                    UI_DEBUG("child branch. Shift focus to child.",id)
                    receiveFocus(root, true);
                    child->shiftFocusTo(e);
                }
            }
        }else{
            //child must be in parent branch
            UI_ERROR("err: uiPage cant be focus target.",id)
            receiveFocus(this,true); //this is correct because this function will relay to the parent
        }
        
    }else{
        UI_ERROR("err: shift-target is NULL",id)
        //do nothing
    }
}

void uiPage::printTree(HardwareSerial * s, String prefix, String suffix){
    s->println(suffix + "" + id + "");
    if(childs.size() > 0){
        for(size_t i = 0; i < childs.size(); ++i){
            uiElement* child = childs.at(i);
            if(i == childs.size() - 1){
                child->printTree(s, prefix + "     ", prefix + "└── ");
            }else{
                child->printTree(s, prefix + "|    ", prefix + "├── ");
            }
        }
    }
}