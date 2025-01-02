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

void uiPage::receiveFocus(uiRoot* sender){
    focus = FocusState::child;
    S_log("uiPage: relay to child",id);
    childWithFocus = focusChild;
    focusChild->receiveFocus(this);
}

void uiPage::receiveFocus(uiElement* sender){
    focus = FocusState::parent;
    S_log("uiPage: relay to parent",id);
    root->receiveFocus();
};

uiClassHirachyType uiPage::getUIClassHirachyType(){
    return uiClassHirachyType::page;
};

void uiPage::resetFocusAndSelection(bool recursive){
    S_log("resetFocusAndSelection",id);
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
                    S_log("child branch. Shift focus to child.",id)
                    receiveFocus(root);
                    child->shiftFocusTo(e);
                }
            }
        }else{
            //child must be in parent branch
            S_log("err: uiPage cant be focus target.",id)
            receiveFocus(this); //this is correct because this function will relay to the parent
        }
        
    }else{
        S_log("err: shift-target is NULL",id)
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