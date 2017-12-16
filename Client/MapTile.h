#ifndef MAPTILE_H
#define MAPTILE_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>




class MapTile : public QGraphicsRectItem
{

public:
    MapTile(int x,int y,int left,int top,int width,int height,QGraphicsItem *parent=nullptr);
    ~MapTile();
    int getX(){return x;}
    int getY(){return y;}
    int getBId(){return bId;}
    void setBId(int bId){this->bId=bId;}
    void setText(QString newText){this->text->setText(newText);}
    int getUniqueBId(){return uniqueBId;}
    void setUniqueBId(int uBiD){this->uniqueBId=uBiD;}
    QString getText(){return this->text->text();}

    void setMainTile(int x,int y){this->mainTileX=x;this->mainTileY=y;}
    void setLargeurBat(int l){this->largeurBat=l;}
    void setHauteurBat(int h){this->hauteurBat=h;}

    int getLargeurBat(){return largeurBat;}
    int getHauteurBat(){return hauteurBat;}
    int getMainTileX(){return mainTileX;}
    int getMainTileY(){return mainTileY;}


    static void setScene(QGraphicsScene *scene);
private:
    int x;
    int y;
    int bId;
    int uniqueBId;

    static QGraphicsScene *scene;
    QGraphicsSimpleTextItem* text;

    int mainTileX;
    int mainTileY;
    int largeurBat;
    int hauteurBat;

};

#endif // MAPTILE_H
