#include "uiBasics.h"

//Position
Position::Position(){
    posX=0;
    posY=0;
};

Position::Position(unsigned int _posX, unsigned int _posY){
    this->setPosition(_posX,_posY);
}

Position::~Position(){

};

unsigned int Position::getX(){
    return posX;
};

unsigned int Position::getY(){
    return posY;
};

void Position::setPosition(unsigned int _posX, unsigned int _posY){
    posX=_posX;
    posY=_posY;
};


void Position::setPosX(unsigned int _posX){
    posX=_posX;
};

void Position::setPosY(unsigned int _posY){
    posY=_posY;
};

UICartesianQuadrant Position::getQuadrant(Position secPos){
    if(posX == secPos.getX() && posY == secPos.getY()){
        return UICartesianQuadrant_UNDEFINED;
    }
    if(posX == secPos.getX()){
        return UICartesianQuadrant_onY;
    }
    if(posY == secPos.getY()){
        return UICartesianQuadrant_onX;
    }
    if(posX < secPos.getX() && posY < secPos.getY()){
        return UICartesianQuadrant_I;
    }
    if(posX > secPos.getX() && posY < secPos.getY()){
        return UICartesianQuadrant_II;
    }
    if(posX > secPos.getX() && posY > secPos.getY()){
        return UICartesianQuadrant_III;
    }
    if(posX < secPos.getX() && posY > secPos.getY()){
        return UICartesianQuadrant_IV;
    }
}

Position Position::getPosition(){
    return Position(posX,posY);
}


//FixedSizing


FixedSizing::FixedSizing(){
    width=0;
    height=0;
};

FixedSizing::FixedSizing(int _width, int _height){
    width=_width;
    height=_height;
};

FixedSizing::~FixedSizing(){

};


int FixedSizing::getWidth(){
    return width;
};

int FixedSizing::getHeight(){
    return height;
};

FixedSizing FixedSizing::getFixedSizing(){
    return FixedSizing(this->width, this->height);
};

//Sizing
Sizing::Sizing(){
    width=0;
    height=0;
};

Sizing::Sizing(int _width, int _height){
    this->setSizing(_width,_height);
}

Sizing::Sizing(FixedSizing fs){
    this->setSizing(fs.getWidth(),fs.getHeight());
}

Sizing::~Sizing(){

};

void Sizing::setSizing(int _width, int _height){
    width=_width;
    height=_height;
};


void Sizing::setWidth(int _width){
    width=_width;
};

void Sizing::setHeight(int _height){
    height=_height;
};

Sizing Sizing::getSizing(){
    return Sizing(this->width, this->height);
};

//Dimension
/*
TODO: Implement getter and setter for width and height in Dimension. The inherited fuctions ignore the 2-Point mode and return wron values.
*/
Dimension::Dimension():Sizing(),Position(){
    treatAs2PointBox = false;
};

Dimension::Dimension(Position _position, Sizing _sizing):Sizing(_sizing),Position(_position){
    treatAs2PointBox = false;
};

Dimension::Dimension(Position _position1, Position _position2):Sizing(_position2.getX(), _position2.getY()),Position(_position1){
    treatAs2PointBox = true;
};

Dimension::Dimension(unsigned int _posX, unsigned int _posY, int _width, int _height, bool _treatAs2PointBox):Sizing(_width,_height),Position(_posX,_posY){
    treatAs2PointBox = _treatAs2PointBox;
};

Dimension::Dimension(Position _position1, FixedSizing _sizing):Sizing(_sizing),Position(_position1){
    treatAs2PointBox = false;
};

Sizing Dimension::getSizing(){
    return Sizing(this->width, this->height);
};

bool Dimension::isBoundingBox(){
    return treatAs2PointBox;
}

Position Dimension::getPosition(UICorner corner){
    if(!treatAs2PointBox){
        switch(corner){
            case UICorner_BL:
                return Position(posX, posY+height);
                break;
            case UICorner_UR:
                return Position(posX+width, posY);
            break;
            case UICorner_BR:
                return Position(posX+width, posY+height);
            break;
            default:
                return Position(posX, posY);
        }
    }else{
        switch(corner){
            case UICorner_BL:
                return Position(posX, height);
                break;
            case UICorner_UR:
                return Position(width, posY);
            break;
            case UICorner_BR:
                return Position(width, height);
            break;
            default:
                return Position(posX, posY);
        }
    }
    
    
}

Position Dimension::getCenter(){
    return Position(this->posX+(width/2),this->posY+(height/2));
}

unsigned int Dimension::getShortestToCenter(){
    if(height>=width){
        return width/2;
    }else{
        return height/2;
    }
}






//Viewport
Viewport::Viewport(Dimension _dimension){
    dimension = _dimension;
}


//Padding
Padding::Padding(){

};
Padding::Padding(unsigned int vertical, unsigned int horizontal){
    top = horizontal;
    bottom = horizontal;
    start = vertical;
    end = vertical;
};
Padding::Padding(unsigned int all){
    top = all;
    bottom = all;
    start = all;
    end = all;
};
Padding::Padding(unsigned int _top, unsigned int _start, unsigned int _bottom, unsigned int _end){
    top = _top;
    bottom = _bottom;
    start = _start;
    end = _end;
};
Dimension Padding::getOuter(Dimension d){
    return Dimension(
        d.getX()-start,//x
        d.getY()-top,//y
        d.getWidth()+start+end,//width
        d.getHeight()+top+bottom//height
    );
};
Dimension Padding::getInner(Dimension d){
    return Dimension(
        d.getX()+start,//x
        d.getY()+top,//y
        d.getWidth()-(start+end),//width
        d.getHeight()-(top+bottom)//height
    );
};
void Padding::setTop(unsigned int _v){
    top = _v;
};
void Padding::setStart(unsigned int _v){
    start = _v;
};
void Padding::setEnd(unsigned int _v){
    end = _v;
};
void Padding::setBottom(unsigned int _v){
    bottom = _v;
};
void Padding::setVertical(unsigned int _v){
    start = _v;
    end = _v;
};
void Padding::setHorizontal(unsigned int _v){
    top = _v;
    bottom = _v;
};
void Padding::setAll(unsigned int _v){
    top = _v;
    bottom = _v;
    start = _v;
    end = _v;
}

unsigned int Padding::getTop(){
    return top;
};

unsigned int Padding::getStart(){
    return start;
};

unsigned int Padding::getEnd(){
    return end;
};

unsigned int Padding::getBottom(){
    return bottom;
};


//Viewport
int Viewport::convertX(int _x){
    return _x + dimension.getX();
}

int Viewport::convertY(int _y){
    return _y + dimension.getY();
}

int Viewport::convertWidth(int _width){
    if(_width + dimension.getX() > dimension.getWidth()){
        return dimension.getX() + dimension.getWidth();
    }else{
        return _width + dimension.getX();
    }
}

int Viewport::convertHeight(int _height){
     if(_height + dimension.getY() > dimension.getHeight()){
        return dimension.getY() + dimension.getHeight();
    }else{
        return _height + dimension.getY();
    }
}

String SelectionModetoString(SelectionMode selectionMode){
    switch(selectionMode){
        case SelectionMode::selectable: return "selectable";
        case SelectionMode::notSelectable: return "notSelectable";
        case SelectionMode::passthroughSelection: return "passthroughSelection";
        case SelectionMode::forwardSelection: return "forwardSelection";
        default: return "unknown";
    }
}

String FocusModetoString(FocusMode focusMode){
    switch(focusMode){
        case FocusMode::target: return "target";
        case FocusMode::collection: return "collection";
        case FocusMode::passthrough: return "passthrough";
        case FocusMode::passive: return "passive";
        default: return "unknown";
    }
}

String FocusStatetoString(FocusState focusState){
    switch(focusState){
        case FocusState::parent: return "parent";
        case FocusState::current: return "current";
        case FocusState::child: return "child";
        default: return "unknown";
    }
}


uiVisualTransformation::uiVisualTransformation(){
    invertedBackground = false;
    invertedContent = false;
}

uiVisualTransformation::uiVisualTransformation(bool _invertedContent, bool _invertedBackground){
    invertedBackground = _invertedBackground;
    invertedContent = _invertedContent;
}

//UserAction
UIEventType getUIEventTypeFromUserAction(UserAction UA){
    switch (UA){
        case UserAction::none: 
            return UIEventType::UIET_unknown;
        case UserAction::backButton:
            return UIEventType::UIET_onBack;
        case UserAction::leftButton:
            return UIEventType::UIET_onLeft;
        case UserAction::rightButton:
            return UIEventType::UIET_onRight;
        case UserAction::enterButton:
            return UIEventType::UIET_onEnter;
        case UserAction::customButton1:
            return UIEventType::UIET_onCustom1;
        case UserAction::customButton2:
            return UIEventType::UIET_onCustom2;
        case UserAction::customButton3:
            return UIEventType::UIET_onCustom3;
        case UserAction::customButton4:
            return UIEventType::UIET_onCustom4;
        case UserAction::backSecondaryButton:
            return UIEventType::UIET_onSecondaryBack;
        case UserAction::leftSecondaryButton:
            return UIEventType::UIET_onSecondaryLeft;
        case UserAction::rightSecondaryButton:
            return UIEventType::UIET_onSecondaryRight;
        case UserAction::enterSecondaryButton:
            return UIEventType::UIET_onSecondaryEnter;
        case UserAction::customSecondaryButton1:
            return UIEventType::UIET_onSecondaryCustom1;
        case UserAction::customSecondaryButton2:
            return UIEventType::UIET_onSecondaryCustom2;
        case UserAction::customSecondaryButton3:
            return UIEventType::UIET_onSecondaryCustom3;
        case UserAction::customSecondaryButton4:
            return UIEventType::UIET_onSecondaryCustom4;
        default:
            return UIEventType::UIET_unknown;
    }
}

//UIEventType
String UIEventTypeToString(UIEventType et){
    switch(et){
        case UIEventType::UIET_unknown: return "UIET_unknown";
        case UIEventType::UIET_recvFocus: return "UIET_recvFocus";
        case UIEventType::UIET_lostFocus: return "UIET_lostFocus";
        case UIEventType::UIET_onEnter: return "UIET_onEnter";
        case UIEventType::UIET_onBack: return "UIET_onBack";
        case UIEventType::UIET_onFocusBounce: return "UIET_onFocusBounce";
        case UIEventType::UIET_onLeft: return "UIET_onLeft";
        case UIEventType::UIET_onRight: return "UIET_onRight";
        case UIEventType::UIET_onCustom1: return "UIET_onCustom1";
        case UIEventType::UIET_onCustom2: return "UIET_onCustom2";
        case UIEventType::UIET_onCustom3: return "UIET_onCustom3";
        case UIEventType::UIET_onCustom4: return "UIET_onCustom4";
        case UIEventType::UIET_onSecondaryBack: return "UIET_onSecondaryBack";
        case UIEventType::UIET_onSecondaryLeft: return "UIET_onSecondaryLeft";
        case UIEventType::UIET_onSecondaryRight: return "UIET_onSecondaryRight";
        case UIEventType::UIET_onSecondaryEnter: return "UIET_onSecondaryEnter";
        case UIEventType::UIET_onSecondaryCustom1: return "UIET_onSecondaryCustom1";
        case UIEventType::UIET_onSecondaryCustom2: return "UIET_onSecondaryCustom2";
        case UIEventType::UIET_onSecondaryCustom3: return "UIET_onSecondaryCustom3";
        case UIEventType::UIET_onSecondaryCustom4: return "UIET_onSecondaryCustom4";
        case UIEventType::UIET_onChange: return "UIET_onChange";
        default: return "UIET_unknown - please implement";
    }
}

String UIEventTypeToStringExplenation(UIEventType et){
    switch(et){
        case UIEventType::UIET_unknown: return "the event is unknown";
        case UIEventType::UIET_recvFocus: return "the element has received focus";
        case UIEventType::UIET_lostFocus: return "the element has lost the focus";
        case UIEventType::UIET_onEnter: return "enter was pressed but not used for navigation";
        case UIEventType::UIET_onBack: return "exit was pressed but not used for navigation";
        case UIEventType::UIET_onFocusBounce: return "the element has received the focus but due to its configuration the focus got bounced";
        case UIEventType::UIET_onLeft: return "left was pressed but not used for naviagation";
        case UIEventType::UIET_onRight: return "right was pressed but not used for navigation";
        case UIEventType::UIET_onCustom1: return "onCustom1 was pressed";
        case UIEventType::UIET_onCustom2: return "onCustom2 was pressed";
        case UIEventType::UIET_onCustom3: return "onCustom3 was pressed";
        case UIEventType::UIET_onCustom4: return "onCustom4 was pressed";
        case UIEventType::UIET_onSecondaryBack: return "secondary back was pressed but not used for navigation";
        case UIEventType::UIET_onSecondaryLeft: return "secondary left was pressed but not used for naviagation";
        case UIEventType::UIET_onSecondaryRight: return "secondary right was pressed but not used for navigation";
        case UIEventType::UIET_onSecondaryEnter: return "secondary enter was pressed but not used for navigation";
        case UIEventType::UIET_onSecondaryCustom1: return "secondary custom1 was pressed";
        case UIEventType::UIET_onSecondaryCustom2: return "secondary custom2 was pressed";
        case UIEventType::UIET_onSecondaryCustom3: return "secondary custom3 was pressed";
        case UIEventType::UIET_onSecondaryCustom4: return "secondary custom4 was pressed";
        case UIEventType::UIET_onChange: return "the element has changed its state";
        default: return "the event is unknown - please implement";
    }
}