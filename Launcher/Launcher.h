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

class QButtonGroup;
class QComboBox;
class QDir;
class QFileDialog;
class QGraphicsScene;
class QGraphicsView;
class QLabel;
class QLineEdit;
class QListWidget;
class QMessageBox;
class QPainter;
class QPixmap;
class QPushButton;
class QSoundEffect;
class QSpinBox;
class QVBoxLayout;

#include <QMainWindow>
#include <QProcess>

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

    QLabel* lMapSize;
    QLabel* lSeed;
    QPushButton* pRandSeed;
    QLabel* lDifficulty;
    QLabel* lGameName;

    QSpinBox* sbMapSize;
    QSpinBox* sbSeed;
    QComboBox* cbDifficulty;
    QLineEdit* leGameName;

    QPushButton* pSetGameFile;
    QPushButton* pSetSaveFolder;

    QPixmap pmPreview;
    QLabel* lbl;
    //Sound
    QSoundEffect *se;

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
    const int randomRange = 10001;
    void updateSaves();
    void updateListWidget(QStringList sl);
    QStringList getSavesList();

    void displayWidgets();

private slots:
    void play();
    void setViewMode(bool b);

    void setGameFile();
    void setSaveFolder();
    void updatePreview();
protected:
    //void timerEvent(QTimerEvent *event);
protected slots:

};
