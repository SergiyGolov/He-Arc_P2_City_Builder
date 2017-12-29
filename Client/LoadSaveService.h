#ifndef LOADSAVESERVICE_H
#define LOADSAVESERVICE_H

#include <QList>

class LoadSaveService : public QObject
{
public:
public slots:
    static void loadGameUI();
    static void saveGameUI();
private:
    static void loadGame(QFile* filename);
    static void saveGame(QFile* filename);
    static void saveOptions();
    static void loadOptions();
};

#endif
