#ifndef GUI_H
#define GUI_H

#include <QGraphicsView>
#include "MapView.h"
#include "PickerElement.h"

class QGraphicsScene;
class ConstantBuilding;

class GuiView : public QGraphicsView
{
    Q_OBJECT

public:
    static GuiView* getGuiView();
    ~GuiView();
    void showBuildingPickerMenu(int tabId);
    int getCurrentTabId(){return activeTabId;}
    void selectBuilding(int n);

private:
    static GuiView* guiViewInstance;
    GuiView(QWidget *parent = 0);
    int activeTabId;
    QGraphicsScene *scene;
    QGraphicsRectItem *tab;
    QGraphicsSimpleTextItem *text;
    std::vector<PickerElement*> *tabText;
    std::vector<PickerElement*> *tabCat;
    int screenWidth;
    int screenHeight;
    PickerElement* road;
    PickerElement* remove;
    int cptMaxCat;


protected:
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // GUI_H
