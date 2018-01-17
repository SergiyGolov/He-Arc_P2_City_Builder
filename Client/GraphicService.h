#ifndef GRAPHICSERVICE_H
#define GRAPHICSERVICE_H

#include "MainWidget.h"
#include <QMessageBox>
#include <QString>


class GraphicService
{

private:
    //constructor
    GraphicService(QString file="");
    static GraphicService* graphicServiceInstance;
    QMessageBox *loading;

public:
    //destructor
    ~GraphicService();
    //methods
    static GraphicService* getGraphicService(QString file="");
    void setKeyboardShortcuts(int key);
    void enableLoadingMessage(){loading->show();}
    void disableLoadingMessage(){loading->hide();}


};
#endif // GRAPHICSERVICE_H
