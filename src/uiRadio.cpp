#include "uiRadio.h"


/*
######################################################
                    uiRadio
######################################################
*/

uiRadio::uiRadio(Position _position, string _text, bool _checked, uiEventCallback _onChange, uiEventCallback _onRcvFocus, Padding _padding){
    visible = true;
    id = "uiRadio";
    selectionMode = SelectionMode::selectable;
    focusMode = FocusMode::collection;
    padding = _padding;
    position = _position;
    text = _text;
    checked = _checked;
    onRcvFocus = _onRcvFocus;
    onChange = _onChange;
}

void uiRadio::receiveFocus(uiElement* sender, bool isPreselection){
    UI_DEBUG("bounce focus",id)
    sender->receiveFocus(this, isPreselection);
    SafeCallback(onRcvFocus, onRcvFocus(this, UIEventType::UIET_onFocusBounce))
    if(!isPreselection){
        checked = !checked;
        SafeCallback(onChange,onChange(this, UIEventType::UIET_onChange))
        SafeCallback(groupParent, groupParent->updateRadioSelection(this, UIEventType::UIET_onChange))
    }
}

void uiRadio::drawThis(frameInfo* f){
    //S_log("draw",id);
    //int height = getTextSizing(f).getHeight();
    int height =  getTextSizing(f).getHeight();

    switch(selected){
        case SelectionState::notSelected:
            //S_log("draw not selected",id)
            drawCircle(
                f,
                uiVisualTransformation(false,false),
                Position(position.getX()+(height/2),position.getY()+(height/2)),
                height/2
            );
            
            drawText(
                f,
                uiVisualTransformation(false,false),
                Position(position.getX()+height+(2*padding.getStart()),position.getY())
            );

            if(checked){
                drawFilledCircle(
                    f,
                    uiVisualTransformation(false,false),
                    Position(position.getX()+(height/2),position.getY()+(height/2)),
                    height/4
                );
            }

        break;
        default:
            //S_log("draw selected",id);
            if(f->highlightSelected){
                drawFilledCircle(
                    f,
                    uiVisualTransformation(false,false),
                    Position(position.getX()+(height/2),position.getY()+(height/2)),
                    height/2
                );
                
                if(checked){
                    drawFilledCircle(
                        f,
                        uiVisualTransformation(true,true),
                        Position(position.getX()+(height/2),position.getY()+(height/2)),
                        height/4
                    );
                }
            }else{
                drawCircle(
                    f,
                    uiVisualTransformation(false,false),
                    Position(position.getX()+(height/2),position.getY()+(height/2)),
                    height/2
                );

                if(checked){
                    drawFilledCircle(
                        f,
                        uiVisualTransformation(false,false),
                        Position(position.getX()+(height/2),position.getY()+(height/2)),
                        height/4
                    );
                }
            }
            drawText(
                    f,
                    uiVisualTransformation(false,false),
                    Position(position.getX()+height+(2*padding.getStart()),position.getY())
                );
    }
}

void uiRadio::setOnRcvFocus(uiEventCallback _onRcvFocus){
    onRcvFocus = _onRcvFocus;
}

void uiRadio::setChecked(bool _checked){
    checked = _checked;
}

bool uiRadio::getChecked(){
    return checked;
}

void uiRadio::setGroupParent(uiRadioGroup* _groupParent){
    groupParent = _groupParent;
}

/*
######################################################
                    uiRadioGroup
######################################################
*/

uiRadioGroup::uiRadioGroup(uiEventCallback _onChange){
    visible = true;
    id = "uiRadioGroup";
    selectionMode = SelectionMode::selectable;
    focusMode = FocusMode::target;
    onChange = _onChange;
}

void uiRadioGroup::addRadio(uiRadio* radio){
    radios.push_back(radio);
    this->addChild(radio);
    radio->setParent(this);
    radio->setGroupParent(this);
    if(checkedRadio>-1){
        radio->setChecked(false);
    }else{
        if(radio->getChecked()){
            checkedRadio = radios.size()-1;
        }
    }
}

void uiRadioGroup::setCheckedRadioIndex(int index){
    if(index < radios.size()){
        checkedRadio = index;
        //this is foolproof but not the most efficient way to do this
        for(uiRadio* r : radios){
            r->setChecked(false);
        }
        radios.at(checkedRadio)->setChecked(true);
    }
}

int uiRadioGroup::getCheckedRadioIndex(){
    return checkedRadio;
}

void uiRadioGroup::updateRadioSelection(void * sender, UIEventType event_type){
    uiRadio* radio = (uiRadio*)sender;
    if(radio->getChecked()){
        int index = 0;
        for(uiRadio* r : radios){
            if(r == radio){
                checkedRadio = index;
            }else{
                r->setChecked(false);
            }
            index++;
        }
        SafeCallback(onChange, onChange(sender, UIEventType::UIET_onChange))
    }
}

void uiRadioGroup::drawThis(frameInfo* f){
    //S_log("draw",id);
    drawChilds(f);
}

void uiRadioGroup::setOnChange(uiEventCallback _onChange){
    onChange = _onChange;
}


