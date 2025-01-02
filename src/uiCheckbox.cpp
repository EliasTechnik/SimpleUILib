#include "uiCheckbox.h"


/*
######################################################
                    uiCheckbox
######################################################
*/

uiCheckbox::uiCheckbox(Position _position, string _text, bool _checked, uiEventCallback _onRcvFocus, Padding _padding){
    visible = true;
    id = "uiCheckbox";
    selectionMode = SelectionMode::selectable;
    focusMode = FocusMode::collection;
    padding = _padding;
    position = _position;
    text = _text;
    checked = _checked;
    onRcvFocus = _onRcvFocus;
}

void uiCheckbox::receiveFocus(uiElement* sender){
    UI_DEBUG("bounce focus",id)
    sender->receiveFocus(this);
    checked = !checked;
    SafeCallback(onRcvFocus, onRcvFocus(this, UIEventType::UIET_onChange))
}

void uiCheckbox::drawThis(frameInfo* f){
    //UI_DEBUG("draw",id);
    //int height = getTextSizing(f).getHeight();
    int height =  getTextSizing(f).getHeight();

    switch(selected){
        case SelectionState::notSelected:
            //UI_DEBUG("draw not selected",id)
            drawFrame(
                f,
                uiVisualTransformation(false,false),
                padding.getOuter(Dimension(position, Sizing(height,height)))
            );
            
            drawText(
                f,
                uiVisualTransformation(false,false),
                Position(position.getX()+height+(2*padding.getStart()),position.getY())
            );

            if(checked){
                drawCheckmark(
                    f,
                    uiVisualTransformation(false,false),
                    Dimension(Position(position.getX()+padding.getStart(),position.getY()+padding.getTop()),Sizing(height-2*padding.getStart(),height-2*padding.getStart()))
                );
            }

        break;
        default:
            //UI_DEBUG("draw selected",id);
            if(f->highlightSelected){
                drawBox(
                    f,
                    uiVisualTransformation(false,false),
                    padding.getOuter(Dimension(position, Sizing(height,height)))
                );
                
                if(checked){
                    drawCheckmark(
                        f,
                        uiVisualTransformation(true,true),
                        Dimension(Position(position.getX()+padding.getStart(),position.getY()+padding.getTop()),Sizing(height-2*padding.getStart(),height-2*padding.getStart()))
                    );
                }
            }else{
                drawFrame(
                    f,
                    uiVisualTransformation(false,false),
                    padding.getOuter(Dimension(position, Sizing(height,height)))
                );

                if(checked){
                    drawCheckmark(
                        f,
                        uiVisualTransformation(false,false),
                        Dimension(Position(position.getX()+padding.getStart(),position.getY()+padding.getTop()),Sizing(height-2*padding.getStart(),height-2*padding.getStart()))
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