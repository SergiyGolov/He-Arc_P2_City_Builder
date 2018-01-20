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

    QLabel* lTitle;                 //Title
    QLabel* lBackgroundImage;       //Used to display the background image

    //Loader/Checker
    QString sPathGameFile;          //Contains the path to gameclient.exe
    QString sPathSaveFolder;        //Contains the path to the saves folder
    QLabel* lGameFile;              //Displays the game client path
    QLabel* lSaveFolder;            //Displays the saves folder path
    QPushButton* pSetGameFile;      //Opens a filedialog to select gameclient.exe
    QPushButton* pSetSaveFolder;    //Opens a folderdialog to select the saves folder

    //Game settings
    QLabel* lMapSize;               //"New Game" menu labels
    QLabel* lSeed;
    QLabel* lDifficulty;
    QLabel* lGameName;
    QSpinBox* sbMapSize;            //Where the desired map size is to be entered
    QPushButton* pRandSeed;         //Generates "random" map seed when clicked on
    QSpinBox* sbSeed;               //Where a custom map seed can be chosen
    QComboBox* cbDifficulty;        //Allows to chose between 4 different game difficulties
    QLineEdit* leGameName;          //Where the game name should be entered
    QListWidget* listSaves;         //Saves list

    //Map preview
    QPixmap pmPreview;              //Contains the map preview image
    QLabel* lPixmap;                //Used to display the QPixmap above

    //Launch menu
    QPushButton* pNewGame;          //Shows the "New game" menu
    QPushButton* pLoadSave;         //Shows the saves list
    QPushButton* pPlay;             //Attempts to enter the game
    QProcess process;               //Launches the game client

private:
    const int randomRange = 10000;  //Used to limit the random generations
    //Functions
    void displayWidgets();          //Regroups all the widgets properties to set at launch
    void connections();             //Regroups signals to slots connections

    void updateInfos();             //Updates the checking tools about save and game folders
    void updateSaves();             //Loads .cbsave files from the file system to the saves list to be able to select them
    void updateListWidget(QStringList sl);
    QStringList getSavesList();

    void randomizeSeed();

private slots:
    void setGameFile();             //Opens a filedialog to select gameclient.exe
    void setSaveFolder();           //Opens a folderdialog to select the saves folder
    void updatePreview();           //Updates the map preview
    void setViewMode(bool b);       //Displays either "New game" or "Load game" view
    void play();                    //Takes infos from the fields and attempt to launch the client
};
