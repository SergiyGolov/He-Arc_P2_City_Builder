#ifndef GRAPHICSERVICE_H
#define GRAPHICSERVICE_H

#include "MainWidget.h"
#include <QMessageBox>


class GraphicService
{

private:
    //constructor
    GraphicService();
    static GraphicService* graphicServiceInstance;
    QMessageBox *loading;

public:
    //destructor
    ~GraphicService();
    //methods
    static GraphicService* getGraphicService();
    void setKeyboardShortcuts(int key);
    void enableLoadingMessage(){loading->show();}
    void disableLoadingMessage(){loading->hide();}







};
#endif // GRAPHICSERVICE_H
