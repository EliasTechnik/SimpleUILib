#include "uiImage.h"

/*
######################################################
                    uiXBM
######################################################
*/
uiImage::uiImage(){
    imgSizing = FixedSizing(0, 0);
}

uiImage::uiImage(int _width, int _height, unsigned char* _img, bool _inverted, bool _transparent){
    inverted = _inverted;
    transparent = _transparent;

    setImage(_width, _height, _img);
}

void uiImage::invertXBM(){
    if(imgInverted == nullptr){
        imgInverted = (unsigned char*)malloc(width*height);
    }

    for(int i = 0; i < width*height; i++){
        imgInverted[i] = ~img[i];
    }
}

void uiImage::setImage(int _width, int _height, unsigned char* _img){
    width = _width;
    height = _height;
    img = _img;

    imgSizing = FixedSizing(width, height);

    if(inverted){
        invertXBM();
    }
}

void uiImage::setInverted(bool _inverted){
    inverted = _inverted;
    if(inverted && imgInverted == nullptr){
        invertXBM();
    }
}

void uiImage::setTransparent(bool _transparent){
    transparent = _transparent;
}

void uiImage::setImage(uiImage* _img){
    setImage(_img->width, _img->height, _img->img);
    setInverted(_img->inverted);
    setTransparent(_img->transparent);
}

void uiImage::drawImage(frameInfo* f, uiVisualTransformation vt, Position p){
    if(!transparent){
        drawBox(f, vt, Dimension(p, getFixedSizing()));
    }
    
    if(vt.invertedContent){
        f->display->setDrawColor(0);
    }else{
        f->display->setDrawColor(1);
    }

    if(inverted){
        f->display->drawXBM(
            f->viewportOffset.convertX(p.getX()),
            f->viewportOffset.convertY(p.getY()), 
            f->viewportOffset.convertWidth(width),
            f->viewportOffset.convertHeight(height),
            imgInverted
        );
    }else{
        f->display->drawXBM(
            f->viewportOffset.convertX(p.getX()),
            f->viewportOffset.convertY(p.getY()), 
            f->viewportOffset.convertWidth(width),
            f->viewportOffset.convertHeight(height),
            img
        );
    }
}


/*
######################################################
                    uiImageContainer
######################################################
*/


uiImageContainer::uiImageContainer(int _x, int _y, int _width, int _height, unsigned char* _img, bool _inverted, bool _transparent):uiImage(_width, _height, _img, _inverted, _transparent){
    posX = _x;
    posY = _y;
}

void uiImageContainer::drawThis(frameInfo* f){
    drawImage(f, uiVisualTransformation(false, false), getPosition());
}