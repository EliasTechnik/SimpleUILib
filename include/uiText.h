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
};


class uiPassiveLabel: public uiCollection, public uiText{
    protected:
        Position position;
        void drawThis(frameInfo* f) override;  
    public:
        uiPassiveLabel();
        uiPassiveLabel(string _text, Position _position, bool isVisible = true);
        //TODO: setFont, setSize, setColor, setUnderline, setItallic, setBold
};

/*
class InteractiveStaticLabel: public uiStaticLabel, public uiInteractive{
    protected:
        void drawThis(frameInfo* f) override;  
    public:
        InteractiveStaticLabel();
        InteractiveStaticLabel(string _text, unsigned int _posX, unsigned int _posY, unsigned int _width, unsigned int _height, bool isVisible = true);
        
};
*/
