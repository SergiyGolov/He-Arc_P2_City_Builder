#include "Client/PickerElement.h"

#include <QGraphicsSimpleTextItem>

PickerElement::PickerElement(int bId,QString textStr, int left, int top, int width, int height,QGraphicsItem* parent):QGraphicsRectItem(left,top,width,height,parent){

    this->bId=bId;

    this->text=new QGraphicsSimpleTextItem(textStr);

    this->setZValue(1);
    this->text->setZValue(0);

   if(PickerElement::scene!=nullptr)PickerElement::scene->addItem(this->text);


}



QGraphicsScene* PickerElement::scene=nullptr;



 void PickerElement::setScene(QGraphicsScene *scene){
     PickerElement::scene=scene;
 }

 PickerElement::~PickerElement(){

 }

 void  PickerElement::bouger(qreal x, qreal y){
     this->setPos(x,y);
     this->text->setPos(x,y);
 }
