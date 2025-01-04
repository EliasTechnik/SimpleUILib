#include "uiElement.h"
#include "uiPage.h"

//uiBaseElement
uiElement::uiElement(){
    visible = true;
};

uiElement::~uiElement(){
    uiElement* element;
    if(childs.size()>0){
        for(unsigned int i=0;childs.size()-1;i++){
            element = childs.at(i);
            delete element;
        }
    }
};

uiElement::uiElement( SelectionMode _selectionMode, bool isVisible) {
    visible = isVisible;
    selectionMode = _selectionMode;
};

void uiElement::setVisible(bool isVisible){
    visible = isVisible;
};

bool uiElement::getVisible(){
    return visible;
};

bool uiElement::setSelected(SelectionState isSelected){ 
    UI_DEBUG("selection update",id);
    bool succes = false;
    switch(focusMode){
        case FocusMode::target:
            UI_WARNING("warn: This element uses target focus but got an selectionState update. This might be an error?",id);
            switch(selectionMode){
                case SelectionMode::selectable:
                    selected = isSelected;
                    return true;
                break;
                case SelectionMode::passthroughSelection:
                    if(childs.size()>0){
                        for(unsigned int i = 0;childs.size()-1; i++){
                            succes = childs.at(i)->setSelected(isSelected) && true;
                        }
                        return succes;
                    }
                break;
                case SelectionMode::forwardSelection:
                    selected = isSelected;
                    if(childs.size()>0){
                        for(unsigned int i = 0;childs.size()-1; i++){
                            childs.at(i)->setSelected(isSelected);
                        }
                    }
                    return true;
                break;
                case SelectionMode::notSelectable:
                    selected = SelectionState::notSelected;
                    return false;
                break;
            }
        break;
        case FocusMode::collection:
            UI_DEBUG("FocusMode::collection",id)
            switch(selectionMode){
                case SelectionMode::selectable:
                    UI_DEBUG("SelectionMode::selectable",id)
                    selected = isSelected;
                    return true;
                break;
                case SelectionMode::passthroughSelection:
                    UI_DEBUG("SelectionMode::passthroughtSelection",id)
                    if(childs.size()>0){
                        for(unsigned int i = 0;childs.size()-1; i++){
                            succes = childs.at(i)->setSelected(isSelected) && true;
                        }
                        return succes;
                    }
                break;
                case SelectionMode::forwardSelection:
                    UI_DEBUG("SelectionMode::forwardSelection",id)
                    selected = isSelected;
                    if(childs.size()>0){
                        for(unsigned int i = 0;childs.size()-1; i++){
                            childs.at(i)->setSelected(isSelected);
                        }
                    }
                    return true;
                break;
                case SelectionMode::notSelectable:
                    UI_DEBUG("SelectionMode::notSelectable",id)
                    selected = SelectionState::notSelected;
                    return false;
                break;
            }
        break;
        case FocusMode::passthrough:
            UI_DEBUG("FocusMode::passthrough",id)
            if(focusChild != nullptr){
                return focusChild->setSelected(isSelected);
            }else{
                UI_ERROR("err: no focusChild to pass selection through.",id)
                return false;
            }
        break;
        case FocusMode::passive:
        UI_DEBUG("FocusMode::passive",id)
            switch(selectionMode){
                case SelectionMode::selectable:
                    selected = isSelected;
                    UI_WARNING("warn: This element cant receive focus but got an selectionState update. This might be an error.",id)
                    return true;
                break;
                case SelectionMode::passthroughSelection:
                    UI_WARNING("warn: This element cant receive focus but got an selectionState update. This might be an error.",id)
                    if(childs.size()>0){
                        for(unsigned int i = 0;childs.size()-1; i++){
                            succes = childs.at(i)->setSelected(isSelected) && true;
                        }
                        return succes;
                    }
                break;
                case SelectionMode::forwardSelection:
                    UI_WARNING("warn: This element cant receive focus but got an selectionState update. This might be an error.",id)
                    selected = isSelected;
                    if(childs.size()>0){
                        for(unsigned int i = 0;childs.size()-1; i++){
                            childs.at(i)->setSelected(isSelected);
                        }
                    }
                    return true;
                break;
                case SelectionMode::notSelectable:
                    selected = SelectionState::notSelected;
                    return false;
                break;
            }
        break;
    }
    UI_ERROR("err: control reached end of function but is not supposed to!",id)
    return false;
};

bool uiElement::getSelectable(){
    if(selectionMode == SelectionMode::notSelectable){
        UI_DEBUG("notSelectable",id)
        return false;
    }else{
        UI_DEBUG("selectable",id)
        return true;
    }
};

void uiElement::setSelectionMode(SelectionMode _selectionMode){
    selectionMode = _selectionMode;
};

SelectionState uiElement::getSelected(){
    return selected;
};

FocusState uiElement::getFocusState(){
    return focus;
}

void uiElement::setFocusMode(FocusMode _mode){
    focusMode = _mode;
}

FocusMode uiElement::getFocusMode(){
    return focusMode;
}

SelectionMode uiElement::getSelectionMode(){
    return selectionMode;
}

void uiElement::setID(String _id){
    id = _id;
}

void uiElement::react(UserAction UA){
    UI_DEBUG("react",id);
    int cid = -2;
    switch(focus){
        case FocusState::child:
            //we hand it down
            if(childWithFocus != nullptr){
                UI_DEBUG("hand down",id);
                childWithFocus->react(UA);
            }else{
                UI_ERROR("err: FocusState is child but no child with focus registered exists!", id);
            }
            break;
        case FocusState::current:
            //we handle it
            UI_DEBUG("i have to react",id);
            switch (UA){
                case UserAction::backButton:
                    //the focus shifts back to parent
                    if(parent != nullptr){
                        focus = FocusState::parent;
                        if(parent != nullptr){
                            selectFocusReceiverMethod(parent);
                            //parent->receiveFocus(this);
                            if(selectedChildID != -1){
                                childs.at(selectedChildID)->setSelected(SelectionState::notSelected);
                            }
                        }else{
                            UI_ERROR("err: element is orphan! (has no parent)",id);
                        }
                        
                        //onLeave()
                    }
                    break;
                case UserAction::enterButton:
                    if(focusMode == FocusMode::collection){
                        //onEnter()
                        UI_DEBUG("Enter pressed. TODO: Callback",id)
                    }else{
                        if(focusMode == FocusMode::target){
                            //child gets focus
                            if(selectedChildID != -1){
                                UI_DEBUG("enter. push focus to child",id)
                                childWithFocus = childs.at(selectedChildID);
                                focus = FocusState::child;
                                childWithFocus->setSelected(SelectionState::notSelected); //remove selection. this is now the problem of the child
                                childWithFocus->receiveFocus(this);
                            }else{
                                UI_ERROR("err: No child selected. TODO?",id); 
                                SafeCallback(onUnassignedInput, onUnassignedInput(this, UIET_onEnter));
                            }
                        }
                    }
                    break;
                case UserAction::leftButton:{
                    //previous child gets selected 
                    if(focusMode == FocusMode::target){
                        UI_DEBUG("left: sel priv child",id)
                        cid = getPriviousSelectableChildID();

                        if(cid == -1){
                            UI_ERROR("err: No selectable child found.",id)
                        }else{
                            //remove selection from old child
                            childs.at(selectedChildID)->setSelected(SelectionState::notSelected);
                            selectedChildID = cid;
                            childs.at(selectedChildID)->setSelected(SelectionState::showAsSelected);
                        }
                    }else{
                        UI_DEBUG("Left pressed. Callback",id)
                        SafeCallback(onUnassignedInput, onUnassignedInput(this, UIET_onLeft));
                        
                    }
                    break;
                }
                case UserAction::rightButton:{
                    //next child gets selected 
                    if(focusMode == FocusMode::target){
                        UI_DEBUG("right: sel next child",id)
                        cid = getNextSelectableChildID();

                        if(cid == -1){
                            UI_ERROR("err: No selectable child found.",id)
                        }else{
                            //remove selection from old child
                            childs.at(selectedChildID)->setSelected(SelectionState::notSelected);
                            selectedChildID = cid;
                            childs.at(selectedChildID)->setSelected(SelectionState::showAsSelected);
                        }
                    }else{
                         UI_DEBUG("Right pressed. Callback",id)
                         if(onUnassignedInput != nullptr){
                            onUnassignedInput(this, UIET_onRight);
                        }
                    }
                    break;
                }
                default:
                    SafeCallback(onUnassignedInput, onUnassignedInput(this, getUIEventTypeFromUserAction(UA)));
                    break;
            }
            break;
        default:
            //this should not happen
            UI_ERROR("err: reacted but Parent has focus!",id)
            break;
    };
};

void uiElement::selectFocusReceiverMethod(uiElement* receiver){
    uiClassHirachyType type = receiver->getUIClassHirachyType();
    
    if(type ==  uiClassHirachyType::page){
        //uiPage* page = dynamic_cast<uiPage*>(receiver);
        uiPage* page = static_cast<uiPage*>(receiver);
        if(page != nullptr){
            page->receiveFocus(this);
        }else{
            UI_ERROR("err: Cast to page failed.",id);
        }
    }else{
        receiver->receiveFocus(this);   
    };
}

void uiElement::setChildSelection(bool ignoreFocusChild){
    //set selection
    if(ignoreFocusChild){
        UI_DEBUG("ignoring focus child",id)
        if(selectedChildID<childs.size() && selectedChildID>=0){
            if(childs.at(selectedChildID)->getSelectable()){
                childs.at(selectedChildID)->setSelected(SelectionState::showAsSelected);
            }else{
                setChildSelection(false);
            }
        }else{
            UI_WARNING("previous selected child index is higher then child count. The index is no longer valid. Fall back to focusChild. (There might be an error in the implementation!)",id)
            setChildSelection(false);
        }
    }else{
        if(focusChild != nullptr){
            if(focusChild->getSelectable()){
                UI_ERROR("err: focusChild is not selectable. Fallback to first selectable child",id)
                uiElement* oldChildWithFocus = childs.at(selectedChildID);
                if(oldChildWithFocus != nullptr){
                    oldChildWithFocus->setSelected(SelectionState::notSelected);
                }
                selectedChildID = getChildIndex(focusChild);
                focusChild->setSelected(SelectionState::showAsSelected);
            }else{
                UI_ERROR("err: focusChild is not selectable. Fallback to first selectable child",id)
                int cid = getNextSelectableChildID();

                if(cid == -1){
                    UI_ERROR("err: No selectable child found. Bouncing to Parent",id)
                    focus = FocusState::parent;
                    selectFocusReceiverMethod(parent);
                }else{
                    //remove selection from old child
                    childs.at(selectedChildID)->setSelected(SelectionState::notSelected);
                    selectedChildID = cid;
                    childs.at(selectedChildID)->setSelected(SelectionState::showAsSelected);
                }
            }
        }else{
            UI_ERROR("err: no focusChild. Fallback to first selectable child",id)
            int cid = getNextSelectableChildID();

            if(cid == -1){
                UI_ERROR("err: No selectable child found. Bouncing to Parent",id)
                focus = FocusState::parent;
                selectFocusReceiverMethod(parent);
                //parent->receiveFocus(this);
            }else{
                //remove selection from old child
                childs.at(selectedChildID)->setSelected(SelectionState::notSelected);
                selectedChildID = cid;
                childs.at(selectedChildID)->setSelected(SelectionState::showAsSelected);
            }
        }
    }

}

void uiElement::receiveFocus(uiElement* sender){ //todo
    UI_DEBUG("received focus",id)
    //check the focus mode
    switch(focusMode){
        case FocusMode::target:
            //check the direction
            switch(focus){
                case FocusState::child:
                    UI_DEBUG("has focus, came from child",id)
                    focus = FocusState::current;
                    //set selction
                    setChildSelection(true); //select previous selected child
                    break;
                case FocusState::current:
                    //this should not happen
                    UI_ERROR("err: Received focus but already had it.",id)
                    break;
                case FocusState::parent:
                    UI_DEBUG("has focus, came from parent",id)
                    focus = FocusState::current;
                    //set selection
                    setChildSelection(false); //select focus child
                    break;
            }
            break;
        case FocusMode::collection:
            //check the direction
            switch(focus){
                case FocusState::child:
                    UI_ERROR("err: Received focus from child but we are running in collection mode. Defaulting to FocusState::current.", id);
                    focus = FocusState::current;
                    this->setSelected(SelectionState::Selected);
                    break;
                case FocusState::current:
                    //this should not happen
                    UI_ERROR("err: Received focus but already had it. Running in Collection mode.", id);
                    focus = FocusState::current;
                    this->setSelected(SelectionState::Selected);
                    break;
                case FocusState::parent:
                    focus = FocusState::current;
                    this->setSelected(SelectionState::Selected);
                    break;
                }
            break;
        case FocusMode::passthrough:
            //check the direction
            switch(focus){
                case FocusState::child:
                    focus = FocusState::parent;
                    selectFocusReceiverMethod(parent);
                    //parent->receiveFocus(this);
                    break;
                case FocusState::current:
                    //this should not happen
                    UI_ERROR("err: Received focus but already had it. Passthrough enabled but direction unknown. Defaulting to parent", id);
                    selectFocusReceiverMethod(parent);
                    //parent->receiveFocus(this);
                    break;
                case FocusState::parent:
                    if(focusChild != nullptr){
                        focus = FocusState::child;
                        focusChild->receiveFocus(this);
                    }else{
                        //default to first child
                        if(childs.size()>0){
                            UI_WARNING("warn: No focusChild set. Defaulting to first child.",id)
                            focus = FocusState::child;
                            childWithFocus = childs.at(0);
                            childs.at(0)->receiveFocus(this);
                        }else{
                            //bounce
                            UI_ERROR("err: No focusChild and no childs to receive focus. Defaulting to parent",id)
                            focus = FocusState::parent;
                            selectFocusReceiverMethod(parent);
                            //parent->receiveFocus(this);
                        }
                    }
                    break;
                }
            break;
        case FocusMode::passive:
            UI_ERROR("err: Received focus but this element is passive. Focus gets bounced.",id)
            selectFocusReceiverMethod(parent);
            //parent->receiveFocus(this);
            break;
    }    
};

void uiElement::setParent(uiElement* _parent){
    parent = _parent;
};

uiElement* uiElement::getParent(){
    return parent;
};

//add a child to the element. If isFocusChild is true the child will be the focusChild which means it will be always the first one to receive focus when focus comes from the parent.
//Because focusChild should not be null we set it to the first added child which is selectable if no focusChild is set.
void uiElement::addChild(uiElement* _child, bool isFocusChild){
    _child->setParent(this);
    childs.push_back(_child);
    if(isFocusChild){
        focusChild = _child;
    }else{
        if(focusChild == nullptr && _child->getSelectable()){
            focusChild = _child;
        }
    }
};

void uiElement::drawChilds(frameInfo* f){
    if(childs.size()>0){
        for (uiElement* e : childs){
            e->draw(f);
        }
    }
}

void uiElement::drawThis(frameInfo* f){
    //do nothing
}

void uiElement::draw(frameInfo* f){
    this->drawThis(f);
    this->drawChilds(f);
}

int uiElement::getNextSelectableChildID(){
    UI_DEBUG("getNextSelectable()",id)
    int index = selectedChildID;
    int allowedChecks = childs.size();
    index++;
    if(index>=childs.size()){
            index = 0;
    }
    uiElement* e;
    do{
        e = childs.at(index);
        if(e != nullptr){
            if(e->getSelectable()){
                return index;
            }else{
                index++;
            }
        }else{
            index++;
        }
        if(index>=childs.size()){
            index = 0;
        }
        allowedChecks--;
    }while(index != selectedChildID && allowedChecks>0);
    UI_ERROR("err: No selectable child found.",id)
    return -1;
}

int uiElement::getPriviousSelectableChildID(){
    UI_DEBUG("getPreviousSelectable()",id)
    int index = selectedChildID;
    index--;
    if(index<0){
        index = childs.size()-1;
    }
    uiElement* e;
    do{
        e = childs.at(index);
        if(e != nullptr){
            if(e->getSelectable()){
                return index;
            }else{
                index--;
            }
        }else{
            index--;
        }
        if(index<0){
            index = childs.size()-1;
        }
    }while(index != selectedChildID);

    return -1;
};

int uiElement::getChildIndex(uiElement* child){
    auto it = find(childs.begin(), childs.end(), child);

    // If element was found
    if (it != childs.end()){
        // calculating the index
        return it - childs.begin();
    }
    else {
        // If the element is not present in the vector
        return -1;
    }
};

String uiElement::getConfig(){ //todo
    return "Config of "+id+":\nfocusMode: "+FocusModetoString(focusMode)+"\nselectMode: "+SelectionModetoString(selectionMode);
};

uiClassHirachyType uiElement::getUIClassHirachyType(){
    return uiClassHirachyType::element;
};

bool uiElement::isInChildBranch(uiElement* e){
    if(e == this){
        return true;
    }else{
        if(childs.size() == 0){
            return false;
        }else{
            for(uiElement* child : childs){
                if(child->isInChildBranch(e)){
                    return true;
                }
            }
            return false;
        }   
    }
};

void uiElement::resetFocusAndSelection(bool recursive){
    UI_DEBUG("resetFocusAndSelection",id)
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

void uiElement::shiftFocusTo(uiElement* e){
    UI_DEBUG("shiftFocusTo",id)
    if(e != nullptr){
        //check if it is this element or a child
        UI_DEBUG("is it me?",id)
        if(e == this){
            UI_DEBUG("focus shifted to me",id)
            receiveFocus(parent);
        }else{
            UI_DEBUG("is in child branch?",id)
            if(isInChildBranch(e)){
                UI_DEBUG("is in child branch",id)
                for(uiElement* child : childs){
                    if(child->isInChildBranch(e)){
                        UI_DEBUG("child branch. Shift focus to child",id)
                        focus = FocusState::child;
                        UI_DEBUG("set focus state",id)
                        if(childWithFocus != nullptr){
                            UI_DEBUG("remove selection",id)
                            childWithFocus->setSelected(SelectionState::notSelected); //remove selection. this is now the problem of the child
                        }
                        UI_DEBUG("set child with focus",id)
                        childWithFocus = child;
                        UI_DEBUG("get child index",id)
                        selectedChildID = getChildIndex(child);
                        UI_DEBUG("shift on child",id)
                        child->shiftFocusTo(e);
                    }
                }
            }else{
                //child must be in parent branch
                UI_DEBUG("parent branch. shift focus to parent",id);
                focus = FocusState::parent;
                childWithFocus->setSelected(SelectionState::notSelected); //remove selection.
                parent->shiftFocusTo(e);
            }
        }
    }else{
        UI_ERROR("err: Element is NULL. Focus not shifted.",id)
    }
};

void uiElement::printTree(HardwareSerial * s, String prefix, String suffix){
    String sel = "";
    if(selected == SelectionState::Selected){
        sel = "#";
    }else{
        if(selected == SelectionState::showAsSelected){
            sel = "+";
        }
    }
    if(focus == FocusState::current){
        s->println(prefix + sel+ "[" + id + "]" + sel);
    }else{
        s->println(suffix + sel +"" + id + "" + sel);
    }
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

String uiElement::getID(){
    return id;
};