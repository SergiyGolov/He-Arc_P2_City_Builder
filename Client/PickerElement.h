#ifndef PICKERELEMENT_H
#define PICKERELEMENT_H


#include <QGraphicsRectItem>
#include <QDebug>
#include <QGraphicsScene>
#include <QColor>


class QGraphicsTextItem;



class PickerElement : public QGraphicsRectItem
{

public:
    PickerElement(int bId=0,QString textStr="",int left=0,int top=0,int width=0,int height=0,QGraphicsItem *parent=nullptr);
    ~PickerElement();
    int getBId(){return bId;}
    void setBId(int bId){this->bId=bId;}
    void setText(QString newText){this->text->setPlainText(newText);}
    QString getText(){return this->text->toPlainText();}


     static void setScene(QGraphicsScene *scene);
    void moveThis(qreal x, qreal y);
    void changeTextColor(QColor c){this->text->setDefaultTextColor(c);}
private:
    int bId;
    static QGraphicsScene *scene;
    QGraphicsTextItem* text;



};




#endif // PICKERELEMENT_H
