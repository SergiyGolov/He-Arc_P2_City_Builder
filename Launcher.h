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
#include <QProcess>
#include <QMessageBox>

class Launcher : public QMainWindow
{
    Q_OBJECT

public:
    Launcher(QWidget *parent = 0);
    ~Launcher();

    //Decoraction
    QLabel* l_title;

    //Loader/Checker
    QString s_path_GameFile;
    QString s_path_SaveFolder;
    QLabel* l_gameFile;
    QLabel* l_saveFolder;
    QPushButton* p_setGameFile;
    QPushButton* p_setSaveFolder;

    //Launch
    QPushButton* p_newgame;
    QPushButton* p_loadsave;
    QListWidget* listWidgetSaves;
    QPushButton* p_play;
    QProcess process;

    //Functions
    void loadSaves();
    void updateInfos();
public slots:

private:
    void updateSaves();
    void updateListWidget(QStringList sl);
    QStringList getSavesList();

private slots:
    void play();
    void setViewMode(bool b);

    void setGameFile();
    void setSaveFolder();

protected:
    void timerEvent(QTimerEvent *event);
protected slots:

};

#endif // LAUNCH_H
