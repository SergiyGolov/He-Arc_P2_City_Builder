#ifndef PICKERELEMENT_H
#define PICKERELEMENT_H


#include <QGraphicsRectItem>
#include <QDebug>
#include <QGraphicsScene>

class QGraphicsSimpleTextItem;



class PickerElement : public QGraphicsRectItem
{

public:
    PickerElement(int bId=0,QString textStr="",int left=0,int top=0,int width=0,int height=0,QGraphicsItem *parent=nullptr);
    ~PickerElement();
    int getBId(){return bId;}
    void setBId(int bId){this->bId=bId;}
    void setText(QString newText){this->text->setText(newText);}
    QString getText(){return this->text->text();}
    void setTextBrush(QBrush brush){this->text->setBrush(brush);}

     static void setScene(QGraphicsScene *scene);
    void bouger(qreal x, qreal y);
private:
    int bId;
    static QGraphicsScene *scene;
    QGraphicsSimpleTextItem* text;



};




#endif // PICKERELEMENT_H
