#include "uiInputs.h"

/*
##########################################################
                    uiCircularDailLogic
##########################################################
*/

uiCircularDailLogic::uiCircularDailLogic(){
    maxValue = 100;
    minValue = 0;
    value = 0;
}

uiCircularDailLogic::uiCircularDailLogic(int _value, int _max, int _min){
    if(_max >= _min){
        maxValue = _max;
        minValue = _min;
    }else{
        maxValue = _min;
        minValue = _max;
    }
    this->setValue(_value);
}

int uiCircularDailLogic::getValue(){
    return value;
}

int uiCircularDailLogic::setValue(int _value){
    if(minValue <= _value && _value <= maxValue){
        value = _value;
    }else{
        if(_value > maxValue){
            value = maxValue;
        }else{
            value = minValue;
        }
    }
    return value;
}

int uiCircularDailLogic::increaseBy(int a, bool allowRollover){
    if(allowRollover){
        int availRange = maxValue - value;
        int range = maxValue - minValue;
        if(a>availRange){
            int add = a - availRange;
            add = add % range;
            value = minValue + add;// - 1; //remove 1 if buggy
        }else{
            value = value + a;
        }
        return value;
        
    }else{
        return setValue(value+a);
    }
}

int uiCircularDailLogic::decreaseBy(int a, bool allowRollover){
    if(allowRollover){
        int availRange = value-minValue;
        int range = maxValue - minValue;
        if(a>availRange){
            int sub = a - availRange;
            sub = sub % range;
            value = maxValue - sub;// + 1; //remove 1 if buggy
        }else{
            value = value - a;
        }
        return value;
    }else{
        return setValue(value-a);
    }
}

int uiCircularDailLogic::changeBy(int a, bool allowRollover){
    if(a>=0){
        return increaseBy(a, allowRollover);
    }else{
        return decreaseBy((a*-1), allowRollover);
    }
}



/*
##########################################################
                    uiIntValueInput
##########################################################
*/

uiIntValueInput::uiIntValueInput(Position _position, int _value, int _max, int _min, uiNotifyCallback _onChange, Padding _padding):uiCircularDailLogic(_value,_max,_min){
    visible = true;
    id = "uiIntValueInput";
    selectionMode = SelectionMode::selectable;
    focusMode = FocusMode::collection;
    padding = _padding;
    position = _position;    
    onChange = _onChange;
    this->setTextType(number);
    this->setText(to_string(value));
}

int uiIntValueInput::getLongestValue(){
    if(minValue < 0 && maxValue < 0){
        return minValue; //the lowest number is probably the longest
    }
    if(minValue > 0 && maxValue > 0){
        return maxValue; //the highest number is probably the longest
    }
    if(minValue < 0 && maxValue > 0){
        int a = abs(minValue);
        if(maxValue < a){
            return minValue;
        }else{
            return maxValue*-1;
        }
    }
}

void uiIntValueInput::drawThis(frameInfo* f){
    //S_log("draw",id);
    Sizing s;
    if(keepMaxWidth){
        s = getTextSizing(f,to_string(getLongestValue()));
    }else{
        s = getTextSizing(f);
    }
    switch(selected){
        case SelectionState::notSelected:
            //S_log("draw not selected",id)
            drawFrame(
                f,
                uiVisualTransformation(false,false),
                padding.getOuter(Dimension(position, s))
            );
            
            drawText(
                f,
                uiVisualTransformation(false,false),
                position
            );

        break;
        default:
            //S_log("draw selected",id);
            if(focus == FocusState::current){
                drawBox(
                    f,
                    uiVisualTransformation(false,false),
                    padding.getOuter(Dimension(position, s))
                );
                drawText(
                    f,
                    uiVisualTransformation(true,true),
                    position
                );
            }else{
                if(f->highlightSelected){
                    drawBox(
                        f,
                        uiVisualTransformation(false,false),
                        padding.getOuter(Dimension(position, s))
                    );
                    drawText(
                        f,
                        uiVisualTransformation(true,true),
                        position
                    );
                }else{
                    drawFrame(
                        f,
                        uiVisualTransformation(false,false),
                        padding.getOuter(Dimension(position, s))
                    );
                    drawText(
                        f,
                        uiVisualTransformation(false,false),
                        position
                    );
                }
            }
            
    }
}

void uiIntValueInput::react(UserAction UA){
    UI_DEBUG("react",id);
    int cid = -2;
    switch(focus){
        case FocusState::current:
            //we handle it
            UI_DEBUG("i have to react",id);
            switch (UA){
                case UserAction::backButton:
                    UI_DEBUG("back pressed",id);
                    //the focus shifts back to parent
                    if(parent != nullptr){
                        focus = FocusState::parent;
                         UI_DEBUG("back to parent",id);
                        selectFocusReceiverMethod(parent);
                    }else{
                            UI_ERROR("err: element is orphan! (has no parent)",id);
                        }
                    break;
                case UserAction::rightButton:{
                    //increase
                    if(value<maxValue){
                        value++;
                    }else{
                        value = minValue;
                    }
                    this->setText(to_string(value));
                    SafeCallback(onChange,onChange(this));
                    break;
                }
                case UserAction::leftButton:{
                    //decrese
                    if(value>minValue){
                        value--;
                    }else{
                        value = maxValue;
                    }
                    this->setText(to_string(value));
                    SafeCallback(onChange,onChange(this));
                    break;
                }
                default:
                    UI_DEBUG("default",id);
                    SafeCallback(onUnassignedInput,onUnassignedInput(this,getUIEventTypeFromUserAction(UA)));
                    break;
            }
            break;
        default:
            //this should not happen
            UI_ERROR("err: reacted but Parent has focus!",id)
            break;
    };
};