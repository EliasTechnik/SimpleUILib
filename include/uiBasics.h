#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <string>

/*
void (*CBonMessage)(void * context, WSMessage msg);
void * CBcontext;

*/
using namespace std; //to use string

enum UIalign{UIAcenter,UIAleft,UIAright,UIAtop,UIAbottom};

enum UIEventType{
    UIET_unknown,   //the event is unknown
    UIET_recvFocus, //the element has received focus
    UIET_lostFocus, //the element has lost the focus
    UIET_onEnter, //enter was pressed but not used for navigation
    UIET_onBack, //back was pressed but not used for navigation
    UIET_onFocusBounce, //the element has received the focus but due to its configuration the focus got bounced
    UIET_onLeft, //left was pressed but not used for naviagation
    UIET_onRight, //right was pressed but not used for navigation
    UIET_onCustom1, //onCustom1 was pressed
    UIET_onCustom2,  //onCustom2 was pressed
    UIET_onCustom3, //onCustom2 was pressed
    UIET_onCustom4,  //onCustom2 was pressed
    UIET_onSecondaryBack, //secondary back was pressed but not used for navigation
    UIET_onSecondaryLeft, //secondary left was pressed but not used for naviagation
    UIET_onSecondaryRight, //secondary right was pressed but not used for navigation
    UIET_onSecondaryEnter, //secondary enter was pressed but not used for navigation
    UIET_onSecondaryCustom1, //secondary custom1 was pressed
    UIET_onSecondaryCustom2, //secondary custom2 was pressed
    UIET_onSecondaryCustom3, //secondary custom3 was pressed
    UIET_onSecondaryCustom4, //secondary custom4 was pressed
    UIET_onChange, //the element has changed its state
    UIET_onClick, //the element was clicked (or pressed)
}; 

String UIEventTypeToString(UIEventType et);
String UIEventTypeToStringExplenation(UIEventType et);

enum UICorner{
    UICorner_UL,
    UICorner_BL,
    UICorner_UR,
    UICorner_BR
};

enum UICartesianQuadrant{
    UICartesianQuadrant_I,
    UICartesianQuadrant_II,
    UICartesianQuadrant_III,
    UICartesianQuadrant_IV,
    UICartesianQuadrant_onX,
    UICartesianQuadrant_onY,
    UICartesianQuadrant_UNDEFINED
};

enum class UserAction{
    none, //none is used as default and will trigger no action. Components should not react on none. This can lead to unexpected behavior.
    backButton,
    leftButton, 
    rightButton, 
    enterButton, 
    backSecondaryButton, 
    leftSecondaryButton, 
    rightSecondaryButton, 
    enterSecondaryButton, 
    customButton1, 
    customButton2,
    customButton3,
    customButton4,
    customSecondaryButton1,
    customSecondaryButton2,
    customSecondaryButton3,
    customSecondaryButton4,
};

UIEventType getUIEventTypeFromUserAction(UserAction UA);

struct tabPos{
    byte horizontalTabId=0;
    byte verticalTabId=0;
};


/*Every UI Element has dimensions defined as rectangle. posX and posY define the upper left corner of that element.*/

class Position{
    protected:
        unsigned int posX;
        unsigned int posY;
    public:
        Position();
        ~Position();
        Position(unsigned int _posX, unsigned int _posY);
        unsigned int getX();
        unsigned int getY();
        void setPosition(unsigned int _posX = 0, unsigned int _posY = 0);
        void setPosX(unsigned int _posX = 0);
        void setPosY(unsigned int _posY = 0);
        Position getPosition();
        UICartesianQuadrant getQuadrant(Position secPos); //returns 1-4 for the quadrant in which secPos sits relative to this Position. If identical UNDEFINED is returned.
};

class FixedSizing{
    protected:
        int width;
        int height;
    public:
        FixedSizing();
        ~FixedSizing();
        FixedSizing(int _width, int _height);
        int getWidth();
        int getHeight();
        FixedSizing getFixedSizing();
};


class Sizing: public FixedSizing{
    protected:

    public:
        Sizing();
        ~Sizing();
        Sizing(int _width, int _height);
        Sizing(FixedSizing fs);
        void setSizing(int _width = 0, int _height = 0);
        void setWidth(int _width = 0);
        void setHeight(int _height = 0);
        Sizing getSizing();
};

class Dimension: public Position, public Sizing{
    protected:
        bool treatAs2PointBox; //if this is true the sizing values will be treated as secondary position
    public:
        Dimension();
        Dimension(Position _position, Sizing _sizing);
        Dimension(Position _position1, FixedSizing _sizing);
        Dimension(Position _position1, Position _position2);
        Dimension(unsigned int _posX, unsigned int _posY, int _width, int _height, bool _isBoundingBox = false);
        Sizing getSizing();
        bool isBoundingBox();
        Position getPosition(UICorner corner = UICorner_UL);
        Position getCenter();
        unsigned int getShortestToCenter();
};


class Padding{
    protected:
        unsigned int top = 1;
        unsigned int start = 1;
        unsigned int end = 1;
        unsigned int bottom = 1;
    public:
        Padding();
        Padding(unsigned int vertical, unsigned int horizontal);
        Padding(unsigned int all);
        Padding(unsigned int _top, unsigned int _start, unsigned int _bottom, unsigned int _end);
        Dimension getOuter(Dimension d);
        Dimension getInner(Dimension d);
        void setTop(unsigned int _v);
        void setStart(unsigned int _v);
        void setEnd(unsigned int _v);
        void setBottom(unsigned int _v);
        void setVertical(unsigned int _v);
        void setHorizontal(unsigned int _v);
        void setAll(unsigned int _v);
        unsigned int getStart();
        unsigned int getEnd();
        unsigned int getTop();
        unsigned int getBottom();
};

#define UI_DEFAULT_PADDING_0a Padding(0)
#define UI_DEFAULT_PADDING_1a Padding(1)
#define UI_DEFAULT_PADDING_2a Padding(2)
#define UI_DEFAULT_PADDING_3a Padding(3)
#define UI_DEFAULT_PADDING_4a Padding(4)
#define UI_DEFAULT_PADDING_5a Padding(5)


struct Viewport{
    Dimension dimension;
    Viewport();
    Viewport(Dimension _dimension);
    int convertX(int _x);
    int convertY(int _y);
    int convertWidth(int _width);
    int convertHeight(int _height);
};

const Viewport DEFAULT_OFFSET = Viewport(Dimension(0,0,128,64));
const Viewport ZERO_OFFSET_128_X_65 = Viewport(Dimension(0,0,128,64));

/*Information about the current frame. Every draw method gets this info. */

struct frameInfo{
    bool highlightSelected = false;
    Viewport viewportOffset = DEFAULT_OFFSET;
    U8G2* display = nullptr;
};


enum class FocusState{
    parent, //the partent element or above has focus
    current, //this element has focus
    child   //on of our childs has focus
};

String FocusStatetoString(FocusState focusState);

enum class FocusMode{
    target, //can receive focus and is selectable
    collection, //can receive focus but ends the focus tree. All of its childs will show selected.
    passthrough, //can receive focus but will passthrough to its child
    passive //can not receive focus
};

String FocusModetoString(FocusMode focusMode);


enum class SelectionMode{
    selectable, //this component can be selected
    passthroughSelection, //this component cant be selected but will forward its selection to its childs
    forwardSelection, //this component can be selected and will forward its selection state to its childs
    notSelectable //this component is not selectable
};

String SelectionModetoString(SelectionMode selectionMode);

enum class uiClassHirachyType{
    element,
    page,
    root
};

enum class SelectionState{notSelected, showAsSelected, Selected};

enum class FocusDirection{fromParent,fromChild,fromUperNeighbour,fromLowerNeighbour};



enum class ScreenState{off, on};


//remove this -->
/*
struct InputAction{
  UserAction action=UserAction::none;
  bool executed=false;
  bool present=false;
};
*/

#define SafeCallback(callback,exec_cb) if(callback != nullptr){exec_cb;}

typedef void (*uiEventCallback)(void *sender, UIEventType event_type);

typedef void (*uiInputCallback)(UserAction action);

typedef void (*uiNotifyCallback)(void *sender);

struct uiCallback{
    void (*CB)(void * context, void *trigger, UIEventType event_type);
    void * CBcontext;
};

struct DisplayConfig{
    unsigned long screenSleepTime = 20000;
    int frameDistance = 16;//66;
    int flashHalfCycle = 250;
    bool useSleep = true;
    U8G2* display = nullptr;
    Viewport viewportOffset = ZERO_OFFSET_128_X_65;
};

struct uiVisualTransformation{
    bool invertedContent = false;
    bool invertedBackground = false;
    //bool showSelection = false;
    uiVisualTransformation();
    uiVisualTransformation(bool _invertedContent, bool _invertedBackground);
    //uiVisualTransformation(bool _invertedContent, bool _invertedBackground, bool _showSelection);
    //uiVisualTransformation(bool _showSelection);
};

//#define DEFAULT_UI_FONT u8g2_font_helvR08_tf
//#define DEFAULT_UI_FONT u8g2_font_6x10_tf
#define DEFAULT_UI_FONT u8g2_font_helvR08_tr