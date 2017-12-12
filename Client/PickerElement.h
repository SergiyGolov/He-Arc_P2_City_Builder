#ifndef PICKERELEMENT_H
#define PICKERELEMENT_H


#include <QGraphicsRectItem>

class QGraphicsSimpleTextItem;


class PickerElement : public QGraphicsRectItem
{

public:
    PickerElement(int bId=0,QString textStr="",int left=0,int top=0,int width=0,int height=0,QGraphicsItem *parent=nullptr);
    ~PickerElement();
    int getBId(){return bId;}
    void setBId(int bId){this->bId=bId;}
    void setText(QString newText){this->text->setText(newText);}

private:
    int bId;

    QGraphicsSimpleTextItem* text;

};




#endif // PICKERELEMENT_H
