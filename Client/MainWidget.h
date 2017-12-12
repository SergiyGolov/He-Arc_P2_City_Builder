#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "GuiView.h"
#include "MapView.h"


//limite le mettre aussi en singleton pour une gestion plus facile depuis n'inmporte où dans le code


class MainWidget : public QWidget
{
    Q_OBJECT

public:
    static MainWidget* getMainWidget();
    ~MainWidget();
    void showBuildingPickerMenu(int tabId);

private:
    MainWidget(QWidget *parent = 0);
    static MainWidget* mainWidgetInstance;
    GuiView *gui;
    MapView *map;

protected:
     void keyPressEvent(QKeyEvent *event);

};

#endif // WIDGET_H
