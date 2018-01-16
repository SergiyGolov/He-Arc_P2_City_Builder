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
#include <QPixmap>
#include <QButtonGroup>

class Launcher : public QMainWindow
{
    Q_OBJECT

public:
    Launcher(QMainWindow *parent = 0);
    ~Launcher();

    //Decoraction
    QLabel* lTitle;

    //Loader/Checker
    QString sPathGameFile;
    QString sPathSaveFolder;
    QLabel* lGameFile;
    QLabel* lSaveFolder;
    QPushButton* pSetGameFile;
    QPushButton* pSetSaveFolder;

    //Launch
    QPushButton* pNewGame;
    QPushButton* pLoadSave;
    QListWidget* listSaves;
    QPushButton* pPlay;
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
