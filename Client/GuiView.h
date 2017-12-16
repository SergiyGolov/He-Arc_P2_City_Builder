#ifndef GUI_H
#define GUI_H

#include <QGraphicsView>
#include "MapView.h"
#include "PickerElement.h"

class QGraphicsScene;
class ConstantBuilding;


//la mettre en singleton pour pouvoir l'appeller avec un getGui depuis n'importe où !

class GuiView : public QGraphicsView
{
    Q_OBJECT

public:
    static GuiView* getGuiView();
    ~GuiView();
    void showBuildingPickerMenu(int tabId);

private:
    static GuiView* guiViewInstance;
    GuiView(QWidget *parent = 0);
    int activeTabId;
    QGraphicsScene *scene;
    QGraphicsRectItem *tab;
    QGraphicsSimpleTextItem *text;
    std::vector<PickerElement*> *tabText;
    int screenWidth;
    int screenHeight;
    PickerElement* road;
    PickerElement* remove;
    int cptMaxCat;

protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // GUI_H
