#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "GuiView.h"
#include "MapView.h"

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
    GuiView *gui;
    MapView *map;

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // WIDGET_H
