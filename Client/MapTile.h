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
    void setBuildingWidth(int l){this->largeurBat=l;}
    void setBuildingHeight(int h){this->hauteurBat=h;}

    int getBuildingWidth(){return largeurBat;}
    int getBuildingHeight(){return hauteurBat;}
    int getMainTileX(){return mainTileX;}
    int getMainTileY(){return mainTileY;}
    int getPixelWidth(){return rect().size().width();}

    QGraphicsPixmapItem* getPix(){return buildImage;}


    static void setScene(QGraphicsScene *scene);

    void addPixMove(QGraphicsPixmapItem *pix);

    void removePixMove();

    void addPix(QGraphicsPixmapItem *pix);

    void addPixRoad(QGraphicsPixmapItem *pix);


    void removePix();

    bool isOccupied(){return bOccupied;}
    void setOccupied(bool b){bOccupied=b;}

    void setPixOpacity(double op){if(buildImage!=nullptr)buildImage->setOpacity(op);}


private:
    bool bOccupied;
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

    bool bPix;

    QGraphicsPixmapItem* buildImage;

};

#endif // MAPTILE_H
