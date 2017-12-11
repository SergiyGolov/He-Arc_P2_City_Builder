#ifndef LAUNCH_H
#define LAUNCH_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QFileDialog>
#include <QDebug>
#include <QListWidget>
#include <QDir>

class Launcher : public QMainWindow
{
    Q_OBJECT

public:
    Launcher(QWidget *parent = 0);
    ~Launcher();

    QLabel* l_title;

    QString s_gamedirectory;

    QPushButton* p_newgame;
    QPushButton* p_loadsave;
    QPushButton* p_setgamedirectory;
    QPushButton* p_play;

    QVBoxLayout* vboxlayout;

    QListWidget* l_saves;

    QImage i_background;

    void loadSaves();

private slots:
    void setGameDirectory();
    void setViewMode(bool b);
};

#endif // LAUNCH_H
