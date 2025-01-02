#include "uiButtons.h"

/*
##########################################################
                    uiBasicButton
##########################################################
*/
uiBasicButton::uiBasicButton(Position _position, string _text, uiEventCallback _onRcvFocus, Padding _padding){
    visible = true;
    id = "uiBasicButton";
    selectionMode = SelectionMode::selectable;
    focusMode = FocusMode::collection;
    padding = _padding;
    position = _position;
    text = _text;
    onRcvFocus = _onRcvFocus;
}

void uiBasicButton::receiveFocus(uiElement* sender){
    UI_DEBUG("bounce focus",id)
    sender->receiveFocus(this);
    onRcvFocus(this, UIEventType::UIET_onFocusBounce);
}


void uiBasicButton::drawThis(frameInfo* f){
    //UI_DEBUG("draw",id);
    switch(selected){
        case SelectionState::notSelected:
            //UI_DEBUG("draw not selected",id)
            drawFrame(
                f,
                uiVisualTransformation(false,false),
                padding.getOuter(Dimension(position, getTextSizing(f)))
            );
            
            drawText(
                f,
                uiVisualTransformation(false,false),
                position
            );

        break;
        default:
            //UI_DEBUG()"draw selected",id);
            if(f->highlightSelected){
                drawBox(
                    f,
                    uiVisualTransformation(false,false),
                    padding.getOuter(Dimension(position, getTextSizing(f)))
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
                    padding.getOuter(Dimension(position, getTextSizing(f)))
                );
                drawText(
                    f,
                    uiVisualTransformation(false,false),
                    position
                );
            }
    }
}

/*
##########################################################
                    uiFixedButton
##########################################################
*/

uiFixedButton::uiFixedButton(Dimension _dimension, string _text, Padding _padding){
    visible = true;
    id = "uiFixedButton";
    selectionMode = SelectionMode::selectable;
    focusMode = FocusMode::collection;
    padding = _padding;
    dimension = _dimension;
    text = _text;
}

void uiFixedButton::receiveFocus(uiElement* sender){

}


void uiFixedButton::drawThis(frameInfo* f){
    //UI_DEBUG("drawFixed",id);
    switch(selected){
        case SelectionState::notSelected:
            //UI_DEBUG("draw not selected",id)
            drawFrame(
                f,
                uiVisualTransformation(false,false),
                dimension
            );
            drawText(
                f,
                uiVisualTransformation(false,false),
                dimension.getPosition()
            );

        break;
        default:
            //UI_DEBUG("draw selected",id);
            if(f->highlightSelected){
                drawBox(
                    f,
                    uiVisualTransformation(false,false),
                    dimension
                );
                drawText(
                    f,
                    uiVisualTransformation(true,true),
                    dimension.getPosition()
                );
            }else{
                drawFrame(
                    f,
                    uiVisualTransformation(false,false),
                    dimension
                );
                drawText(
                    f,
                    uiVisualTransformation(false,false),
                    dimension.getPosition()
                );
            }
    }
}

/*
##########################################################
                    uiIconButton
##########################################################
*/

uiIconButton::uiIconButton(Position _position, uiImage* _img, uiEventCallback _onRcvFocus, Padding _padding){
    visible = true;
    id = "uiIconButton";
    selectionMode = SelectionMode::selectable;
    focusMode = FocusMode::collection;
    padding = _padding;
    position = _position;
    setImage(_img);
    
    onRcvFocus = _onRcvFocus;
}

void uiIconButton::receiveFocus(uiElement* sender){
    UI_DEBUG("bounce focus",id)
    sender->receiveFocus(this);
    onRcvFocus(this, UIEventType::UIET_onFocusBounce);
}


void uiIconButton::drawThis(frameInfo* f){
    //UI_DEBUG("draw",id);
    switch(selected){
        case SelectionState::notSelected:
            //UI_DEBUG("draw not selected",id)
            drawFrame(
                f,
                uiVisualTransformation(false,false),
                padding.getOuter(Dimension(position, imgSizing))
            );

            drawImage(
                f,
                uiVisualTransformation(false,false),
                position
            );

        break;
        default:
            //UI_DEBUG("draw selected",id);
            if(f->highlightSelected){
                /*
                drawBox(
                    f,
                    uiVisualTransformation(false,false),
                    padding.getOuter(Dimension(position, getTextSizing(f)))
                );
                */
                drawFrame(
                    f,
                    uiVisualTransformation(false,false),
                    padding.getOuter(Dimension(position, imgSizing))
                );

                drawImage(
                    f,
                    uiVisualTransformation(true,true),
                    position
                );
            }else{
                drawFrame(
                    f,
                    uiVisualTransformation(false,false),
                    padding.getOuter(Dimension(position, imgSizing))
                );
                drawImage(
                    f,
                    uiVisualTransformation(false,false),
                    position
                );
            }
    }
}