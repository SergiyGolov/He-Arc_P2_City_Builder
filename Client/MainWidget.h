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

#include <QWidget>
#include "Client/GuiView.h"
#include "Client/MapView.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    static MainWidget* getMainWidget();
    ~MainWidget();
    void showBuildingPickerMenu(int tabId);

private:
    static MainWidget* mainWidgetInstance;
    MainWidget(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *event);
};
