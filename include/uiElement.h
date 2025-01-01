#pragma once
#include "uiBasics.h"
#include "helper.h"
//#include "uiPage.h" //circluar dependency
#include <vector>

/*uielement is one baseclass for every ui element. It handles attributes common for all ui elements.

uiElements are linked in a tree like structure. Every element can have a parent or a child element.

*/

//class uiPage;


class uiElement{
    protected:
        bool visible = true;
        uiElement* focusChild = nullptr;
        String id = "";
        SelectionState selected = SelectionState::notSelected;
        SelectionMode selectionMode = SelectionMode::notSelectable;
        FocusMode focusMode = FocusMode::passive;
        FocusState focus = FocusState::parent;
        uiElement* parent = nullptr;
        std::vector<uiElement*> childs;
        //unsigned int firstElementToFocus = 0;
        unsigned int selectedChildID = 0;
        uiElement* childWithFocus = nullptr;
        int getNextSelectableChildID();
        int getPriviousSelectableChildID();
        //uiCallback onEnter;
        //uiCallback onLeave;
        uiEventCallback onUnassignedInput = nullptr; //gets called on any input apart from any input with a function assigned. (The assignement can vary on the configuration)
        virtual void drawThis(frameInfo* f); //should be overwritten by every child class which inherrits from uielement
        void drawChilds(frameInfo* f);
        int getChildIndex(uiElement* child);
        void setChildSelection(bool ignoreFocusChild);
        virtual void selectFocusReceiverMethod(uiElement* receiver);
    public:
        uiElement();
        ~uiElement();
        uiElement(SelectionMode _selectionMode, bool isVisible);
        void setVisible(bool isVisible);
        bool getVisible();
        bool setSelected(SelectionState isSelected);
        SelectionState getSelected();
        void setSelectionMode(SelectionMode _selectionMode);
        SelectionMode getSelectionMode();
        bool getSelectable();
        void setFocusMode(FocusMode _mode);
        FocusMode getFocusMode();
        void setParent(uiElement* _parent);
        uiElement* getParent();
        void addChild(uiElement* _child, bool isFocusChild = false);
        //virtual uiElement* getChild();
        void draw(frameInfo* f); 
        virtual void react(UserAction UA);
        virtual void receiveFocus(uiElement* sender);
        FocusState getFocusState();
        void setID(String _id);
        String getConfig();
        virtual uiClassHirachyType getUIClassHirachyType();
};
