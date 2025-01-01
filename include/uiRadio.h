#pragma once

#include "uiBasics.h"
#include "uiElement.h"
#include "helper.h"
#include "uiGraphics.h"
#include "uiPrebuilds.h"
#include "uiText.h"

class uiRadioGroup;


class uiRadio: public uiSelectableCollection, public uiText, public uiCircle, public uiFilledCircle{
    protected:
        Padding padding = UI_DEFAULT_PADDING_2a;
        Position position;
        bool checked = false;
        void drawThis(frameInfo* f) override;
        uiEventCallback onRcvFocus;
        uiRadioGroup* groupParent = nullptr;
    public:
        uiRadio(Position _position, string _text, bool _checked, uiEventCallback _onRcvFocus = nullptr, Padding _padding = UI_DEFAULT_PADDING_2a);
        void setOnRcvFocus(uiEventCallback _onRcvFocus);
        void setChecked(bool _checked);
        bool getChecked();
        void setGroupParent(uiRadioGroup* _groupParent);
        void receiveFocus(uiElement* sender) override;
};

class uiRadioGroup: public uiSelectGroup{
    protected:
        std::vector<uiRadio*> radios;
        int checkedRadio = -1;
        void drawThis(frameInfo* f) override;
        uiEventCallback onChange;
    public:
        uiRadioGroup(uiEventCallback _onChange = nullptr);
        void updateRadioSelection(void * sender, UIEventType event_type);
        void setOnChange(uiEventCallback _onChange);
        void addRadio(uiRadio* radio);
        void setCheckedRadioIndex(int index);
        int getCheckedRadioIndex();
};