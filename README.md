# SimpleUI

This project aims to provide a simple object-oriented UI system for small monochrome embedded screens. The main features of this framework are:

- Easy setup: Build a customizable UI with a few lines of code and with minimal logic.
- Expandable: Need a component which isn't allready provided by the library? No problem. Almost all components are build upon the same base classes. Building a custom component which integrates into the framework is easy.
- Streamline the setup: Because the UI is build during runtime it is easy to streamline the building process by grouping building steps and putting them in build functions.

## Structure

The library itself relies heavily on inheritance but this is only important to know if you wish to write your own components. During runtime, the ui uses a tree structure as the backbone to function. In this tree, ui components have a parent component and might have multiple children components. In the following explanation, this structure is called _UI-Tree_, and the parents and children are called _uiElement_ or elements. 

**For component developers:**
The ```uiElement``` is the base class for each component. It provides the necessary logic to make the backbone of the uiTree work. It does not provide any positional or dimensional data. When writing custom components, they have to inherit ```uiElement```to work within the UI-Tree. See [Writing custom components](#Writing_custom_components) for more information.

## UI-Tree

This is a sample structure for a possible UI tree. It starts at the uiRoot object. The uiRoot receives all inputs and starts the drawing function. It also provides a paging functionality.  uiGroup inherits from uiElement but does things differently. uiGroup does not contain a single child but multiple and can manage them. Because uiRoot doesn't inherit from uiElement there is a need for a glue object between uiRoot and uiElement. This is uiPage.

When building an ui we only add child objects. This means each object only knows its child(s). The parent pointer is learned during runtime.
```

uiRoot
├── uiPage
│   └── uiGroup
│       ├── uiElement
│       ├── uiElement
│       ├── uiGroup
│       │   ├── uiElement
│       │   └── ...
│       └── ...
├── uiPage
│   └── uiElement
└── uiPage
    └── ...
```

## Wording

- Focus: In the ui-tree only one object can have the focus. Focus means that this element processes user inputs.
- Selection: Multiple uiElements can be selected. A selection is usually shown by inverting or flashing. Selections are made by the parent object and shown by the child. A child can deny or forward selections based on selectable and the focus mode.

## Input Mapping

The Framework is designed for four buttons: Up, Down, Enter, Return. Additionaly


## Basics

### UIalign

### UIEventType

### tabPos

### dimensions

Dimensions is a base class and provides positional and size information.

### Viewport

Holds information about the display device and possible restrictions. Viewport is part of the frameInfo

### frameInfo

Holds global information about the current frame. It is used during the drawing cycle.

### FocusState

This is internal and used to determine the focus position in the ui-hirachy.

### FocusMode

FocusMode can be set for each uiElement and controls how the element reacts to focus changes. The following modes are available:

- target: The element can receive focus and is selectable. It handles the selection of its children.
- collection: The element can receive focus and is selectable. It ends the focus tree.
- passthrough: The element can receive focus but will passthrough to its focus-child. If there is no child the focus will bounce and may lead to a crash.
- passive: The element can not receive focus. If it does, the focus will bounce.

## Example

Look at the file ```teachingExample.cpp``` it explains the basic usage of SimpleUI and showcases some of its functions. 
