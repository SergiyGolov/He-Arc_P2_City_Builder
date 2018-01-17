#ifndef LOADSAVESERVICE_H
#define LOADSAVESERVICE_H

#include <QList>

struct Params
{
    int size;
    int difficulty;
    int seed;
    QString name;
    Params()
    {
        size = 100;
        difficulty = 1;
        seed = rand() % 100;
        name = "No name specified";
    }
};

class LoadSaveService : public QObject
{
public:
public slots:
    static void loadGameUI();
    static void saveGameUI();

    static void loadGame(int argc, char *argv[]);
    static void loadGame(QString filename);

private:
    static void saveGame(QFile* filename);
    static void saveOptions();
    static void loadOptions();
    static Params* parse(int argc, char *argv[]);
};

#endif
