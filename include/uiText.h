#pragma once

#include "uiBasics.h"
#include "uiElement.h"
#include "uiText.h"
#include "uiInteractive.h"
#include "uiPrebuilds.h"

enum TextType{
    genericText,
    number
};

class uiText{
    protected:
        string text;
        UIalign align;
        TextType textType;
        const uint8_t* font = DEFAULT_UI_FONT;
        void drawText(frameInfo* f, uiVisualTransformation vt, Position p);
        Sizing getTextSizing(frameInfo* f, string _text); 
    public:
        uiText();
        uiText(string _text);
        uiText(string _text, UIalign _align);
        void setAlign(UIalign _align);
        UIalign getAlign();
        void setText(string _text);
        void setFont(const uint8_t* _font = DEFAULT_UI_FONT);
        void setTextType(TextType _type);
        Sizing getTextSizing(frameInfo* f); 
        string getText();
        TextType getTextType();
        const uint8_t* getFont();
};


class uiLabel: public uiCollection, public uiText{
    protected:
        Position position;
        void drawThis(frameInfo* f) override;  
    public:
        uiLabel();
        uiLabel(string _text, Position _position, bool isVisible = true);
};


