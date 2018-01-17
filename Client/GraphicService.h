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

#include "Client/MainWidget.h"

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
