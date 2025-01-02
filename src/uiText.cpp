
#include "uiText.h"

//uiText

uiText::uiText(){
    text = "";
    align = UIalign::UIAleft;
    textType = genericText;
}

uiText::uiText(string _text){
    text = _text;
    align = UIalign::UIAleft;
    textType = genericText;
}

uiText::uiText(string _text, UIalign _align){
    text = "";
    align = _align;
    textType = genericText;
}

void uiText::setAlign(UIalign _align){
    align = _align;
};

UIalign uiText::getAlign(){
    return align;
}

void uiText::setFont(const uint8_t* _font){
    font = _font;
}

void uiText::setText(string _text){
    text = _text;
}

void uiText::setTextType(TextType _type){
    textType = _type;
}

void uiText::drawText(frameInfo* f, uiVisualTransformation vt, Position p){
    f->display->setFont(font);
    f->display->setFontPosBaseline();
    int height = f->display->getAscent();
    f->display->setFontMode(1); //https://github.com/olikraus/u8g2/wiki/u8g2reference#setdrawcolor
    if(vt.invertedContent){
        f->display->setDrawColor(0);
    }else{
        f->display->setDrawColor(1);
    }
    
    f->display->drawStr(f->viewportOffset.convertX(p.getX()),f->viewportOffset.convertY(p.getY()+height), text.c_str());
}

Sizing uiText::getTextSizing(frameInfo* f, string _text){
    f->display->setFont(font);
    Sizing s;
    f->display->setFontPosBaseline();
    switch(textType){
        case number:
            s = Sizing(f->display->getStrWidth(_text.c_str()),(f->display->getAscent()));
        break;
        default:
            s = Sizing(f->display->getStrWidth(_text.c_str()),(f->display->getAscent())-(f->display->getDescent()));
    }
    return s;
};

Sizing uiText::getTextSizing(frameInfo* f){
    return getTextSizing(f,text);
};

string uiText::getText(){
    return text;
}

TextType uiText::getTextType(){
    return textType;
}

const uint8_t* uiText::getFont(){
    return font;
}


//uiPassiveLabel

uiLabel::uiLabel(){
    //setDimension();
    visible = true;
    selectionMode = SelectionMode::notSelectable;
    focusMode = FocusMode::passive;
    focus = FocusState::parent;
    id = "uiPassiveLabel";
    align = UIalign::UIAleft;
};


uiLabel::uiLabel(string _text, Position _position, bool isVisible){
    position = _position;
    selectionMode = SelectionMode::notSelectable;
    focusMode = FocusMode::passive;
    focus = FocusState::parent;
    visible = true;
    id = "uiStaticLabel";
    text = _text;
    align = UIalign::UIAleft;
};


void uiLabel::drawThis(frameInfo* f){
    //Slog("draw2");
    bool showSelected = (selected == SelectionState::showAsSelected || selected == SelectionState::Selected);

    if(visible){
        drawText(f, uiVisualTransformation(false,false), position);
    }
}

