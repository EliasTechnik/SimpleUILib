#pragma once
#include "uiBasics.h"
#include "uiElement.h"
#include "helper.h"

/*

uiGraphics.h provides all classes to draw basic geometric shapes. 
They can be used when building uiElements or as drawing functions for uiGraphicCanvas  
The uiElement has to inherit the class it wants to use

*/

/*
uiGraphicCanvas is a special uiElemennt which can be used 
to draw shapes on screen at runtime without defining a childclass of uiElement.
It does this by using a list of uiGraphicElements. uiGraphicElement is a dynamic wrapper
which stores Dimensional information about its element and is used to interface with objects inherited from uiGraphic.
*/

//mother object 
class uiGraphic{
    protected:

    public:
        uiGraphic();
        virtual void drawUIGraphic(frameInfo* f, uiVisualTransformation vt, Dimension d);
        
};

//wrapper object 
class uiGraphicElement{
    protected:
        Dimension gInfo;
        uiGraphic* gElement;
        bool color;
        bool visible;
    public:
        uiGraphicElement();
        uiGraphicElement(uiGraphic* graphic, Dimension d);
        uiGraphicElement(uiGraphic* graphic, Dimension d, bool _color, bool _visible);
        void draw(frameInfo* f, uiVisualTransformation vt);
        void setDimension(Dimension d);
        void setGraphic(uiGraphic* e);
        void setColor(bool _color);
        void setVisible(bool _visible);
        //todo get functions
};

//a canvas to display various graphic elements. It can be integrated into the ui-tree.
//The default constructor reasembles the behavior of uiCollection. This however can be changed.
class uiGraphicCanvas: public uiElement{
    protected:
        std::vector<uiGraphicElement*> graphics;
        void drawThis(frameInfo* f) override;
    public:
        uiGraphicCanvas();
        uiGraphicCanvas(bool _visible, String _id, SelectionMode _selectionMode, FocusMode _focusMode);
        void addGraphic(uiGraphicElement* graphic);
};


class uiBox: public uiGraphic{
    protected:
        void drawBox(frameInfo* f, uiVisualTransformation vt, Dimension d);
    public:
        uiBox();
        void drawUIGraphic(frameInfo* f, uiVisualTransformation vt, Dimension d) override;
};

class uiFrame: public uiGraphic{
    protected:
        unsigned int borderWidth = 1;
        void drawFrame(frameInfo* f, uiVisualTransformation vt, Dimension d);
    public:
        uiFrame();
        uiFrame(unsigned int _borderWidth);
        void setBorderWidth(unsigned int _borderWidth);
        void drawUIGraphic(frameInfo* f, uiVisualTransformation vt, Dimension d) override;
};

class uiCircle: public uiGraphic{
    protected:
        unsigned int borderWidth = 1;
        void drawCircle(frameInfo* f, uiVisualTransformation vt, Position center, unsigned int r);
    public:
        uiCircle();
        uiCircle(unsigned int _borderWidth);
        void setBorderWidth(unsigned int _borderWidth);
        void drawUIGraphic(frameInfo* f, uiVisualTransformation vt, Dimension d) override;
};

class uiFilledCircle: public uiGraphic{
    protected:
        void drawFilledCircle(frameInfo* f, uiVisualTransformation vt, Position center, unsigned int r);
    public:
        uiFilledCircle();
        void drawUIGraphic(frameInfo* f, uiVisualTransformation vt, Dimension d) override;
};

class uiLine: public uiGraphic{
    protected:
        unsigned int borderWidth = 1;
        void drawLine(frameInfo* f, uiVisualTransformation vt, Position pos1, Position pos2);
    public:
        uiLine();
        uiLine(unsigned int _borderWidth);
        void setBorderWidth(unsigned int _borderWidth);
        void drawUIGraphic(frameInfo* f, uiVisualTransformation vt, Dimension d) override;
};

class uiCheckmark: public uiGraphic{
    protected:
        void drawCheckmark(frameInfo* f, uiVisualTransformation vt, Dimension d);
        void drawUIGraphic(frameInfo* f, uiVisualTransformation vt, Dimension d) override;
    public:
        uiCheckmark();
};

