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

#include "Client/MapView.h"
#include "Client/PickerElement.h"

#include <QGraphicsView>

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
