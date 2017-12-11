#ifndef MAPTILE_H
#define MAPTILE_H

#include <QGraphicsRectItem>




class MapTile : public QGraphicsRectItem
{

public:
    MapTile(int x,int y,int left,int top,int width,int height,QGraphicsItem *parent=nullptr);
    ~MapTile();
    int getX(){return x;}
    int getY(){return y;}
private:
    int x;
    int y;


};

#endif // MAPTILE_H
