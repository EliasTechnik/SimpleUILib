#include "uiPrebuilds.h"

uiSelectGroup::uiSelectGroup(){
    visible = true;
    id = "uiSelectGroup";
    selectionMode = SelectionMode::selectable;
    focusMode = FocusMode::target;
}

uiSelectableCollection::uiSelectableCollection(){
    visible = true;
    id = "uiCollection";
    selectionMode = SelectionMode::selectable;
    focusMode = FocusMode::collection;
}

uiCollection::uiCollection(){
    visible = true;
    id = "uiCollection";
    selectionMode = SelectionMode::notSelectable;
    focusMode = FocusMode::passive;
}