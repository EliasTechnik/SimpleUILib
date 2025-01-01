
#include "uiGraphics.h"

//uiGraphic

uiGraphic::uiGraphic(){

};




//uiGraphicElement
uiGraphicElement::uiGraphicElement(){
    gInfo.setSizing(0,0);
    gInfo.setPosition(0,0);
    color = true;
    visible = true;
    gElement = nullptr;
};

uiGraphicElement::uiGraphicElement(uiGraphic* element, Dimension d){
    gInfo = d;
    gElement = element;
    color = true;
    visible = true;
}

uiGraphicElement::uiGraphicElement(uiGraphic* element, Dimension d, bool _color, bool _visible){
    gInfo = d;
    gElement = element;
    color = _color;
    visible = _visible;
};

void uiGraphicElement::draw(frameInfo* f, uiVisualTransformation vt){
    if(gElement != nullptr && visible){
        if(!color){
            vt.invertedBackground = !vt.invertedBackground;
            vt.invertedContent = !vt.invertedContent;
        }
        gElement->drawUIGraphic(f,vt,gInfo);
    }
};

void uiGraphicElement::setDimension(Dimension d){
    gInfo = d;
};

void uiGraphicElement::setGraphic(uiGraphic* e){
    gElement = e;
};

void uiGraphicElement::setColor(bool _color){
    color = _color;
};

void uiGraphicElement::setVisible(bool _visible){
    visible = _visible;
};

//uiGraphicCanvas
uiGraphicCanvas::uiGraphicCanvas(){
    visible = true;
    id = "uiGraphicCanvas";
    selectionMode = SelectionMode::notSelectable;
    focusMode = FocusMode::passive;
};

uiGraphicCanvas::uiGraphicCanvas(bool _visible, String _id, SelectionMode _selectionMode, FocusMode _focusMode){
    visible = _visible;
    id = _id;
    selectionMode = _selectionMode;
    focusMode = _focusMode;
};


void uiGraphicCanvas::addGraphic(uiGraphicElement* graphic){
    graphics.push_back(graphic);
};

void uiGraphicCanvas::drawThis(frameInfo* f){
    if(visible){
        uiVisualTransformation vt;
        if(selected == SelectionState::showAsSelected){
            vt.invertedBackground = f->highlightSelected;
            vt.invertedContent = f->highlightSelected;
        }else{
            if(selected == SelectionState::Selected){
                vt.invertedBackground = true;
                vt.invertedContent = true;
            }else{
                vt.invertedBackground = false;
                vt.invertedContent = false;
            }
        }

        for(int i=0; i<graphics.size();i++){
            graphics.at(i)->draw(f,vt);
        }
    }  
}


//uiBox
uiBox::uiBox(){
}

void uiBox::drawBox(frameInfo* f, uiVisualTransformation vt, Dimension d){
    //Slog("draw box")

    if(vt.invertedBackground){
        f->display->setDrawColor(0);
    }else{
        f->display->setDrawColor(1);
    }

    f->display->drawBox(
        f->viewportOffset.convertX(d.getX()),
        f->viewportOffset.convertY(d.getY()), 
        f->viewportOffset.convertWidth(d.getWidth()),
        f->viewportOffset.convertHeight(d.getHeight())
    );
}

void uiBox::drawUIGraphic(frameInfo* f, uiVisualTransformation vt, Dimension d){
    drawBox(f,vt,d);
}

//uiFrame
uiFrame::uiFrame(){
    borderWidth = 1;
}

uiFrame::uiFrame(unsigned int _borderWidth){
    borderWidth = _borderWidth;
}
void uiFrame::setBorderWidth(unsigned int _borderWidth){
    borderWidth = _borderWidth;
}

void uiFrame::drawFrame(frameInfo* f, uiVisualTransformation vt, Dimension d){
    
    if(vt.invertedBackground){
        f->display->setDrawColor(0);
    }else{
        f->display->setDrawColor(1); 
    }
    
    for(byte i = 0;i<=borderWidth-1;i++){
        //Slog("draw frame")

        f->display->drawFrame(
                f->viewportOffset.convertX(d.getX()+i),
                f->viewportOffset.convertY(d.getY()+i), 
                f->viewportOffset.convertWidth(d.getWidth()-i),
                f->viewportOffset.convertHeight(d.getHeight()-i)
            );
    }
}

void uiFrame::drawUIGraphic(frameInfo* f, uiVisualTransformation vt, Dimension d){
    drawFrame(f,vt,d);
}



/*
######################################################
                    uiCircle
######################################################
*/

uiCircle::uiCircle(){
    borderWidth = 1;
}

uiCircle::uiCircle(unsigned int _borderWidth){
    borderWidth = _borderWidth;
}
void uiCircle::setBorderWidth(unsigned int _borderWidth){
    borderWidth = _borderWidth;
}

void uiCircle::drawCircle(frameInfo* f, uiVisualTransformation vt, Position center, unsigned int r){
    if(vt.invertedBackground){
        f->display->setDrawColor(0);
    }else{
        f->display->setDrawColor(1); 
    }
    
    if(borderWidth>1){
        //draw outer circle
        f->display->drawDisc(
            f->viewportOffset.convertX(center.getX()),
            f->viewportOffset.convertY(center.getY()), 
            f->viewportOffset.convertWidth(r)
        );

        //invert color for inner circle
        if(vt.invertedBackground){
            f->display->setDrawColor(1);
        }else{
            f->display->setDrawColor(0); 
        }

        //draw inner circle
        f->display->drawDisc(
            f->viewportOffset.convertX(center.getX()),
            f->viewportOffset.convertY(center.getY()), 
            f->viewportOffset.convertWidth(r-borderWidth)
        );
    }else{
        f->display->drawCircle(
                        f->viewportOffset.convertX(center.getX()),
                        f->viewportOffset.convertY(center.getY()), 
                        f->viewportOffset.convertWidth(r)
                    );
    }
}

void uiCircle::drawUIGraphic(frameInfo* f, uiVisualTransformation vt, Dimension d){
    drawCircle(f,vt,d.getCenter(),d.getShortestToCenter());
}


/*
######################################################
                    uiFilledCircle
######################################################
*/

uiFilledCircle::uiFilledCircle(){
}

void uiFilledCircle::drawFilledCircle(frameInfo* f, uiVisualTransformation vt, Position center, unsigned int r){
    //Slog("draw FilledCircle")

    if(vt.invertedBackground){
        f->display->setDrawColor(0);
    }else{
        f->display->setDrawColor(1);
    }

    f->display->drawDisc(
        f->viewportOffset.convertX(center.getX()),
        f->viewportOffset.convertY(center.getY()), 
        f->viewportOffset.convertWidth(r)
    );
}

void uiFilledCircle::drawUIGraphic(frameInfo* f, uiVisualTransformation vt, Dimension d){
    drawFilledCircle(f,vt,d.getCenter(),d.getShortestToCenter());
}


/*
######################################################
                    uiLine
######################################################
*/
uiLine::uiLine(){
    borderWidth = 1;
}

uiLine::uiLine(unsigned int _borderWidth){
    borderWidth = _borderWidth;
}
void uiLine::setBorderWidth(unsigned int _borderWidth){
    borderWidth = _borderWidth;
}

void uiLine::drawLine(frameInfo* f, uiVisualTransformation vt, Position pos1, Position pos2){
    
    if(vt.invertedBackground){
        f->display->setDrawColor(0);
    }else{
        f->display->setDrawColor(1); 
    }
    
    int offset = borderWidth/2;

    /*
    TODO:
    Drawing angled lines with multiple strokes (borderWidth>1) involves some trigonometry for which i didnt have the time for now. 
    Add it later. For now only horizontal or vertical lines support line width. 
    
    */
    switch(pos1.getQuadrant(pos2)){
        case UICartesianQuadrant_onX:
            for(byte i = 0;i<=borderWidth-1;i++){
                f->display->drawLine(
                        f->viewportOffset.convertX(pos1.getX()),
                        f->viewportOffset.convertY(pos1.getY()+i-offset), 
                        f->viewportOffset.convertWidth(pos2.getX()),
                        f->viewportOffset.convertHeight(pos2.getY()+i-offset)
                    );
            }
            break;
        case UICartesianQuadrant_onY:
            for(byte i = 0;i<=borderWidth-1;i++){
                f->display->drawLine(
                        f->viewportOffset.convertX(pos1.getX()+i-offset),
                        f->viewportOffset.convertY(pos1.getY()), 
                        f->viewportOffset.convertWidth(pos2.getX()+i-offset),
                        f->viewportOffset.convertHeight(pos2.getY())
                    );
            }
            break;
        case UICartesianQuadrant_I:
            f->display->drawLine(
                        f->viewportOffset.convertX(pos1.getX()),
                        f->viewportOffset.convertY(pos1.getY()), 
                        f->viewportOffset.convertWidth(pos2.getX()),
                        f->viewportOffset.convertHeight(pos2.getY())
                    );
            /*
            for(byte i = 0;i<=borderWidth-1;i++){
                f->display->drawLine(
                        f->viewportOffset.convertX(pos1.getX()+i-offset),
                        f->viewportOffset.convertY(pos1.getY()), 
                        f->viewportOffset.convertWidth(pos2.getX()+i)-offset,
                        f->viewportOffset.convertHeight(pos2.getY())
                    );
            }
            */
            break;
        case UICartesianQuadrant_II:
            f->display->drawLine(
                        f->viewportOffset.convertX(pos1.getX()),
                        f->viewportOffset.convertY(pos1.getY()), 
                        f->viewportOffset.convertWidth(pos2.getX()),
                        f->viewportOffset.convertHeight(pos2.getY())
                    );
            /*
            for(byte i = 0;i<=borderWidth-1;i++){
                f->display->drawLine(
                        f->viewportOffset.convertX(pos1.getX()+i-offset),
                        f->viewportOffset.convertY(pos1.getY()), 
                        f->viewportOffset.convertWidth(pos2.getX()+i)-offset,
                        f->viewportOffset.convertHeight(pos2.getY())
                    );
            }
            */
            break;
        case UICartesianQuadrant_III:
            f->display->drawLine(
                        f->viewportOffset.convertX(pos1.getX()),
                        f->viewportOffset.convertY(pos1.getY()), 
                        f->viewportOffset.convertWidth(pos2.getX()),
                        f->viewportOffset.convertHeight(pos2.getY())
                    );
            /*
            for(byte i = 0;i<=borderWidth-1;i++){
                f->display->drawLine(
                        f->viewportOffset.convertX(pos1.getX()+i-offset),
                        f->viewportOffset.convertY(pos1.getY()), 
                        f->viewportOffset.convertWidth(pos2.getX()+i)-offset,
                        f->viewportOffset.convertHeight(pos2.getY())
                    );
            }
            */
            break;
        case UICartesianQuadrant_IV:
            f->display->drawLine(
                        f->viewportOffset.convertX(pos1.getX()),
                        f->viewportOffset.convertY(pos1.getY()), 
                        f->viewportOffset.convertWidth(pos2.getX()),
                        f->viewportOffset.convertHeight(pos2.getY())
                    );
            /*
            for(byte i = 0;i<=borderWidth-1;i++){
                f->display->drawLine(
                        f->viewportOffset.convertX(pos1.getX()+i-offset),
                        f->viewportOffset.convertY(pos1.getY()), 
                        f->viewportOffset.convertWidth(pos2.getX()+i)-offset,
                        f->viewportOffset.convertHeight(pos2.getY())
                    );
            }
            */
            break;
        default:
            f->display->drawPixel(
                        f->viewportOffset.convertX(pos1.getX()),
                        f->viewportOffset.convertY(pos1.getY())
                    );
            break;
    }
}

void uiLine::drawUIGraphic(frameInfo* f, uiVisualTransformation vt, Dimension d){
    drawLine(f,vt,d.getPosition(),d.getPosition(UICorner_BR));
};


/*
######################################################
                    uiCheckmark
######################################################
*/

uiCheckmark::uiCheckmark(){

}

void uiCheckmark::drawCheckmark(frameInfo* f, uiVisualTransformation vt, Dimension d){
    int x = d.getX();
    int y = d.getY();
    int w = d.getWidth();
    int h = d.getHeight();
    int spacing = w/10;

    Position p1 = Position(x+spacing, y+h/2);
    Position p2 = Position(x+w/2, y+h-spacing);
    Position p3 = Position(x+w-spacing, y+spacing);

    if(vt.invertedBackground){
        f->display->setDrawColor(0);
    }else{
        f->display->setDrawColor(1);
    }

    f->display->drawLine(p1.getX(), p1.getY(), p2.getX(), p2.getY());
    f->display->drawLine(p2.getX(), p2.getY(), p3.getX(), p3.getY());
}

void uiCheckmark::drawUIGraphic(frameInfo* f, uiVisualTransformation vt, Dimension d){
    drawCheckmark(f, vt, d);
}