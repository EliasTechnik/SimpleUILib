#pragma once
#include "uiBasics.h"
#include "helper.h"
#include "uiElement.h"

/*
Group in which the elements can be individualy selected.
*/
class uiSelectGroup: public uiElement{
    public:
        uiSelectGroup();
};

/*
Collection which can be selected but all childs cant receive focus. 
This Prebuild is usefull when building uiComponents without group selection logic.
*/
class uiSelectableCollection: public uiElement{
    public:
        uiSelectableCollection();
};

/*
Collection which can not be selected. 
This can be used to group static elements.
*/
class uiCollection: public uiElement{
    public:
        uiCollection();
};