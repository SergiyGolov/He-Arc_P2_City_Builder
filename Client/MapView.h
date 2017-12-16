#ifndef MAPVIEW
#define MAPVIEW

#include <QGraphicsView>
#include <MapTile.h>
#include <QList>
class QTimer;

#define nbcases 256

class GuiView;


class QGraphicsScene;

//clic droit => annule posage de bâtiment !!!

class MapView : public QGraphicsView
{
    Q_OBJECT

public:

    ~MapView();
    void picker(int bId);
    static MapView* getMapView();
    void zoomMeth(bool plusMinus); //TODO
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
    int pixelParCase;
    bool tilesBool[nbcases][nbcases];
    MapTile* tiles[nbcases][nbcases];
    int largeurBat;
    int hauteurBat;
    int rayonBat;
    bool road;
    QList<MapTile*> *rayonTile;
    QList<MapTile*> *tempRoad;
    QList<MapTile*> *tempRemove;
    QColor prevRemoveColor;

    int roadStartX;
    int roadStartY;
    bool grille;
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
