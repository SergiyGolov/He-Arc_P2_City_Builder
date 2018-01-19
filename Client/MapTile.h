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
    void setMainTile(int x,int y){this->mainTileX=x;this->mainTileY=y;}
    void setBuildingWidth(int l){this->buildingWidth=l;}
    void setBuildingHeight(int h){this->buildingHeight=h;}
    int getBuildingWidth(){return buildingWidth;}
    int getBuildingHeight(){return buildingHeight;}
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
    void setDelimiterRect(int x,int y,int w,int h);
    void unsetDelimiterRect();
    void setPixZValue(double z){if(buildImage!=nullptr)buildImage->setZValue(z);}

private:
    bool bOccupied;
    int x;
    int y;
    int bId;
    int uniqueBId;
    QGraphicsRectItem *delimiterRect;
    static QGraphicsScene *scene;
    QGraphicsSimpleTextItem* text;

    int mainTileX;
    int mainTileY;
    int buildingWidth;
    int buildingHeight;

    bool bPix;

    QGraphicsPixmapItem* buildImage;

};
