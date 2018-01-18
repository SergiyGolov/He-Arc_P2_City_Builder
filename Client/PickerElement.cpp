#include "PickerElement.h"
#include <QGraphicsTextItem>

PickerElement::PickerElement(int bId,QString textStr, int left, int top, int width, int height,QGraphicsItem* parent):QGraphicsRectItem(left,top,width,height,parent)
{
    this->bId=bId;

    this->text=new QGraphicsTextItem(textStr);

    this->setZValue(1);

    this->setPen(QPen(Qt::lightGray));
    this->setBrush(QBrush(Qt::lightGray));
    this->text->setZValue(2);
    QFont font(QString("Segoe UI Semilight"),10);
    font.setWeight(40);
    this->text->setFont(font);
    if(PickerElement::scene!=nullptr)PickerElement::scene->addItem(this->text);
}

QGraphicsScene* PickerElement::scene=nullptr;

void PickerElement::setScene(QGraphicsScene *scene)
{
    PickerElement::scene=scene;
}

PickerElement::~PickerElement()
{

}

void  PickerElement::moveThis(qreal x, qreal y)
{
    this->setPos(x,y);
    this->text->setPos(x,y);
}
