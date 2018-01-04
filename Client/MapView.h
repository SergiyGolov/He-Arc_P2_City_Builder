#ifndef MAPVIEW
#define MAPVIEW

#include <QGraphicsView>
#include <MapTile.h>
#include <QList>

#define nbTiles 256

class QTimer;
class GuiView;
class QGraphicsScene;
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
private:
    static MapView* mapViewInstance;
    MapView(QWidget *parent = 0);
    QGraphicsScene *scene;
    double zoom;
    int screenWidth;
    int screenHeight;
    GuiView *gui;
    bool click;
    bool bPicker;
    int pickerBId;
    QColor prevColor;
    MapTile *prevRect;
    int pixelPerTile;
    bool tilesBool[nbTiles][nbTiles];
    MapTile* tiles[nbTiles][nbTiles];
    QColor baseColors[nbTiles][nbTiles];
    int buildWidth;
    int buildHeight;
    int buildRadius;
    bool road;
    QList<MapTile*> *radiusTiles;
    QList<MapTile*> *tempRoad;
    QList<MapTile*> *tempRemove;
    QColor prevRemoveColor;

    Cell* cells;

    int roadStartX;
    int roadStartY;
    bool grid;
    int buildingCount;
    int roadDir;

    bool checkIfNearRoad(MapTile* tile);

    void blinkTileRed(MapTile* tile);

    QTimer *timer;
    MapTile* blinkRedTile;


    bool timeTick;
    int nbTimeTick;
public slots:
    void blinkRedTileSlot();
protected:

    void wheelEvent(QWheelEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // WIDGET_H
