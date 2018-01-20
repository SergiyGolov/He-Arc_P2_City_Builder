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

#include <QGraphicsView>
#include <Client/MapTile.h>
#include <QList>
#include <QIcon>

class QTimer;
class GuiView;
class QGraphicsScene;
class QGraphicsEllipseItem;
struct Cell;

class MapView : public QGraphicsView
{
    Q_OBJECT

public:
    static MapView* getMapView();
    ~MapView();
    void callPicker(int bId);
    void zoomMeth(bool plusMinus);
    void translateMeth(int direction);
    void removeBuildingMode();
    void addRoadMode();
    void toggleGrid();
    int getNbTiles(){return nbTiles;}
    void setNbTiles(int nb){this->nbTiles=nb;}
    double getZoomFactor(){return zoomFactor;}
    void setZoomFactor(double zoom){this->zoomFactor=zoom;}
    int getCameraX(){return cameraX;}
    void setCameraX(int camX){this->cameraX=camX;}
    int getCameraY(){return cameraY;}
    void setCameraY(int camY){this->cameraY=camY;}
    void generateMap();
    void addBuildingFromSave(int id, int x, int y, int angle);
    void toggleAllBuildingRadius();

private:
    double translateFactor;
    double scaleFactor;
    int cameraX;
    int cameraY;
    int nbTiles;
    static MapView* mapViewInstance;
    MapView(QWidget *parent = 0);
    QGraphicsScene *scene;
    double zoomFactor;
    int screenWidth;
    int screenHeight;
    int lastbId;
    int circleWidth;
    bool bPicker;
    int pickerBId;
    QColor prevColor;
    MapTile *prevRect;
    int pixelPerTile;
    QVector<MapTile*> *tiles;
    QVector<QColor> *baseColors;
    int buildWidth;
    int buildHeight;
    int buildRadius;
    bool bRoad;
    QList<MapTile*> *tempRoadList;
    QList<MapTile*> *tempRemoveList;
    QColor prevRemoveColor;
    MapTile* lastTilePix;
    Cell* cells;
    int roadStartX;
    int roadStartY;
    bool bGrid;
    int buildingCount;
    int roadDir;
    bool falseRoadAdd;
    bool checkIfNearRoad(MapTile* tile);
    void blinkTileRed(MapTile* tile);
    QTimer *timer;
    MapTile* blinkRedTile;
    bool bTimeTick;
    int nbTimeTick;
    QGraphicsPixmapItem *currentBuild;
    void moveAddBuilding(MapTile* rect);
    void moveRemoveBuilding(MapTile *rect);
    void moveAddRoad(MapTile* rect);
    void finalAddRoad(MapTile* rect);
    void finalAddBuilding(MapTile* rect);
    void finalRemove(MapTile* rect);
    void cancelAdd(MapTile* rect);
    int countNeighbourRoads(MapTile* tile);
    void updateNeighbourRoad(MapTile* tile);
    void getNeighbours(MapTile* tile);
    QList<MapTile*> *neighbourList;
    QGraphicsEllipseItem *radiusCircle;
    bool bRadius;
    QList<QGraphicsEllipseItem*> *radiusList;
    void showRadius(MapTile* rect);
    void setRadiusCircle(QGraphicsEllipseItem* radius,int bId,int x,int y);

    const char* texRoad = ":/ressources/route.png";
    const char* texRoadL = ":/ressources/routeL.png";
    const char* texRoadT = ":/ressources/routeT.png";
    const char* texRoadX = ":/ressources/routeX.png";

    const char* texHouse = ":/ressources/house.png";
public slots:
    void blinkRedTileSlot();
protected:
    void wheelEvent(QWheelEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};
