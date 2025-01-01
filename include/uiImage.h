#pragma once
#include "uiBasics.h"
#include "helper.h"
#include "uiPrebuilds.h"
#include "uiGraphics.h"

/*

uiImage.h provides all classes to handle bitmaps.
They alone are usless but can be inherited when building uiElements.

*/

/*
TODO:
    - implement inverted
    - implement transparent
*/



class uiImage: public FixedSizing, public uiBox{
    protected:
        bool inverted = false;
        bool transparent = false;
        unsigned char* img = nullptr;
        unsigned char* imgInverted = nullptr;
        FixedSizing imgSizing;
        void drawImage(frameInfo* f, uiVisualTransformation vt, Position p);
        void invertXBM();
    public:
        uiImage();
        uiImage(int _width, int _height, unsigned char* _img = nullptr, bool _inverted = false, bool _transparent = false);
        void setImage(int _width, int _height, unsigned char* _img);
        void setImage(uiImage* img);
        void setInverted(bool _inverted);
        void setTransparent(bool _transparent);
};


class uiImageContainer: public uiImage, public uiCollection, public Position{
    protected:
        void drawThis(frameInfo* f) override;
    public:
        uiImageContainer(int _x, int _y, int _width, int _height, unsigned char* _img = nullptr, bool _inverted = false, bool _transparent = false);
};