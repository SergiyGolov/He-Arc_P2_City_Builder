#include "PickerElement.h"
#include <QGraphicsSimpleTextItem>

PickerElement::PickerElement(int bId,QString textStr, int left, int top, int width, int height,QGraphicsItem* parent):QGraphicsRectItem(left,top,width,height,parent){

    this->bId=bId;
    this->text=new QGraphicsSimpleTextItem(textStr,this);

}


PickerElement::~PickerElement(){

}
