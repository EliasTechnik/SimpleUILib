#pragma once

#include "uiBasics.h"
#include "uiElement.h"
#include "uiPrebuilds.h"
#include "uiText.h"
#include "uiGraphics.h"

class uiCircularDailLogic{
    protected:
        int maxValue = 100; //inclusive
        int minValue = 0; //inclusive
        int value = 42;
        int increaseBy(int a, bool allowRollover = true);
        int decreaseBy(int a, bool allowRollover = true);
    public:
        uiCircularDailLogic();
        uiCircularDailLogic(int _value, int _max, int _min);
        int getValue();
        int setValue(int _value); //will set the value if it is within range. If its out of range the min/max value will be used. 
        int changeBy(int _amount, bool allowRollover = true); //changes the value (add/sub) based on _amount. If allowRollover ist set to true the value will rollover. 

};


class uiIntValueInput: public uiText, public uiBox, public uiFrame, public uiSelectableCollection, public uiCircularDailLogic{
    protected:
        Padding padding = UI_DEFAULT_PADDING_2a;
        Position position;
        bool keepMaxWidth = true;
        void drawThis(frameInfo* f) override;
        int getLongestValue();
        uiNotifyCallback onChange = nullptr;
    public:
        uiIntValueInput(Position _position, int _value, int _max, int _min, uiNotifyCallback _onChange, Padding _padding = UI_DEFAULT_PADDING_2a);
        void setKeepMaxWidth(bool keep); //if set to true (default) the input doesnt shrink when displaying lower numbers than _max.
        void react(UserAction UA) override;
};