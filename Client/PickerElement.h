/*
 * ************************************* Project *************************************
 * School              : Haute Ecole Arc
 * Module              : Projet P2
 * Teachers            : - Beurret Stéphane <Stephane.Beurret@he-arc.ch>
 *                       - Grunenwald David <David.Grunenwald@he-arc.ch>
 * Group               : CityBuilder
 * Group participant   : - Goloviatinski Sergiy <sergiy.goloviatinski@he-arc.ch>
 *                       - Margueron Raphael <raphael.margueron@he-arc.ch>
 *                       - Petroff Damian <damian.petroff@he-arc.ch>
 * ************************************ File spec ************************************
 * Workpackage manager :
 * Description         :
 * ***********************************************************************************
 */
#pragma once

#include <QGraphicsRectItem>
#include <QDebug>
#include <QGraphicsScene>
#include <QColor>

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
    void changeTextColor(QColor c){this->text->setPen(QPen(c));}
private:
    int bId;
    static QGraphicsScene *scene;
    QGraphicsSimpleTextItem* text;



};
