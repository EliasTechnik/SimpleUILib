#pragma once

#include "uiBasics.h"
#include "uiElement.h"
#include "helper.h"
#include "uiGraphics.h"
#include "uiPrebuilds.h"
#include "uiText.h"




class uiCheckbox: public uiSelectableCollection, public uiText, public uiBox, public uiFrame, public uiCheckmark {
    protected:
        Padding padding = UI_DEFAULT_PADDING_2a;
        Position position;
        bool checked = false;
        void drawThis(frameInfo* f) override;
        uiEventCallback onRcvFocus;
    public:
        uiCheckbox(Position _position, string _text, bool _checked, uiEventCallback _onRcvFocus, Padding _padding = UI_DEFAULT_PADDING_2a);
        void receiveFocus(uiElement* sender) override;
};