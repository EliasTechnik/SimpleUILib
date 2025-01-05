#pragma once

#include "uiBasics.h"
#include "uiElement.h"
#include "uiPrebuilds.h"
#include "uiGraphics.h"
#include "uiText.h"
#include "uiImage.h"

class uiBasicButton: public uiSelectableCollection, public uiText, public uiBox, public uiFrame {
    protected:
        Padding padding = UI_DEFAULT_PADDING_2a;
        Position position;
        void drawThis(frameInfo* f) override;
        uiEventCallback onRcvFocus;     //gets called every time the element receives focus
        uiEventCallback onUserPressed;  //gets called when the user presses the button
    public:
        uiBasicButton(Position _position, string _text, uiEventCallback _onUserPressed, uiEventCallback _onRcvFocus = nullptr, Padding _padding = UI_DEFAULT_PADDING_2a);
        void receiveFocus(uiElement* sender, bool isPreselection) override;
};

class uiFixedButton: public uiSelectableCollection, public uiText, public uiBox, public uiFrame {
    protected:
        Padding padding = UI_DEFAULT_PADDING_2a;
        Dimension dimension;
        void drawThis(frameInfo* f) override;
    public:
        uiFixedButton(Dimension _dimension, string _text, Padding _padding = UI_DEFAULT_PADDING_2a);
        void receiveFocus(uiElement* sender, bool isPreselection) override;
};

class uiIconButton: public uiSelectableCollection, public uiImage, public uiFrame {
    protected:
        Padding padding = UI_DEFAULT_PADDING_2a;
        Position position;
        void drawThis(frameInfo* f) override;
        uiEventCallback onRcvFocus;
        uiEventCallback onUserPressed;
    public:
        uiIconButton(Position _position, uiImage* _img, uiEventCallback _onUserPressed, uiEventCallback _onRcvFocus = nullptr, Padding _padding = UI_DEFAULT_PADDING_2a);
        void receiveFocus(uiElement* sender, bool isPreselection) override;
};




