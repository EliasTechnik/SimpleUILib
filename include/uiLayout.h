#pragma once
#include <Arduino.h>
#include <string>
#include "uiBasics.h"
#include "uiElement.h"

/*
Classes found here are responsible to create (dynamic) layouts 
by positioning its elements automaticly. 
They are transparent to the uiNavigation.
*/

/*
The idea is the following:
    - uiLayoutModifier can be configured to do a specific layout
        - rowMode
        - ColumnMode
        - space around
        - space between
        - align start
        - align end
        - align top
        - align bottom
    - uiLayoutModifier can be added to every uiElement which also inhertited from the uiLayoutModifierReceiver
    - during draw() it is used to reposition the childs
        - it does this by taking the vector of childs and doing calculation based upon that
*/


/*
-----------------     WARNING     -----------------
This is clearly a WIP file. At the moment I have not the capacity to think about it.
Developing a Layout system which integrates seamless into the current dynamic OOP aproach
is not the easiest task. I will come back to this eventualy but for now the layout may rest 
and has to be done manualy.
*/



//elements who want to be included into a layout should inherit this
class uiLayoutProvider{
    protected:

    public:
        uiLayoutProvider();


};


class uiLayoutReceiver: public uiElement{
    protected:

    public:
        uiLayoutReceiver();
};


class uiLayoutModifier{
    protected:

    public:
        uiLayoutModifier();

};
