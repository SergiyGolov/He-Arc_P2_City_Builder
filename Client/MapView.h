#ifndef MAPVIEW
#define MAPVIEW

#include <QGraphicsView>
#include <MapTile.h>

#define nbcases 256

class GuiView;


class QGraphicsScene;

//la mettre en singleton pour pouvoir l'appeller avec un getMap depuis n'importe où !

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
    bool tiles[nbcases][nbcases];


protected:

    void wheelEvent(QWheelEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // WIDGET_H
