#ifndef MAPVIEW
#define MAPVIEW

#include <QGraphicsView>
#include <MapTile.h>
#include <QList>



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

private:
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
    bool bClick;
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
    QList<QGraphicsPixmapItem*> *buildPixList;
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


    bool timeTick;
    int nbTimeTick;

    QGraphicsPixmapItem *currentBuild;

    bool checkIfNearRoadY(MapTile* tile);
    bool checkIfNearRoadX(MapTile* tile);

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

public slots:
    void blinkRedTileSlot();
protected:

    void wheelEvent(QWheelEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // WIDGET_H
