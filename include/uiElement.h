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
        bool visible = true; //wether the element is visible or not (applies also to childs)
        uiElement* focusChild = nullptr; //the child wich will be the first to receive focus when focus comes from the parent. If no focusChild is set the first selectable child will be the focusChild.
        String id = ""; //the id of the element. This is used for debugging and for callbacks.
        SelectionState selected = SelectionState::notSelected; //the state of selection
        SelectionMode selectionMode = SelectionMode::notSelectable; //how the element reacts to selection changes
        FocusMode focusMode = FocusMode::passive; //how the element reacts to focus changes
        FocusState focus = FocusState::parent; //wether the parent, this or the child has focus
        uiElement* parent = nullptr; //the parent element
        std::vector<uiElement*> childs; //the child elements
        unsigned int selectedChildID = 0; //the numerical id of the selected child
        uiElement* childWithFocus = nullptr; //the child element which has focus
        int getNextSelectableChildID(); //returns the numerical id of the next selectable child. If no child is selectable it returns -1
        int getPriviousSelectableChildID(); //returns the numerical id of the privious selectable child. If no child is selectable it returns -1
        //uiCallback onEnter;
        //uiCallback onLeave;
        uiEventCallback onUnassignedInput = nullptr; //gets called on any input apart from any input with a function assigned. (The assignement can vary on the configuration)
        virtual void drawThis(frameInfo* f); //should be overwritten by every child class which inherrits from uielement
        void drawChilds(frameInfo* f); //draws all childs
        int getChildIndex(uiElement* child); //returns the numerical id of the given child. If the child is not found it returns -1
        void setChildSelection(bool ignoreFocusChild); //sets the selection of the child elements. If ignoreFocusChild is true the focusChild will be ignored.
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
        void resetFocusAndSelection(bool recursive = false); //resers focus and selection to the default values. This is used when the focus is shifted to another element.
        void shiftFocusTo(uiElement* e); //this function shifts the focus (and selection) to the given element. This can be used to jump to an element.
        uiElement* getElementByID(String _id); //this function returns the element with the given ID. If no element with the given ID is found it returns nullptr. 
        //At the moment there is no logic for duplicate ids. So the first element with the given ID is returned.
        uiElement* getElementByID(String _id, uiElement* sender);
        bool isInChildBranch(uiElement* e);
        void setOnUnassignedInput(uiEventCallback cb);
        void printTree(HardwareSerial * s, String prefix, String suffix); //prints the ui tree to the given serial
};
