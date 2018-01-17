#include "Launcher.h"

#include <QSoundEffect>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>


Launcher::Launcher(QMainWindow *parent) : QMainWindow(parent)
{
    /*
    se = new QSoundEffect(this); //Doesnt work when executed in the debugger
    se->setSource(QUrl::fromLocalFile("./CityBuilderLauncher.wav"));
    se->setLoopCount(QSoundEffect::Infinite);
    se->setVolume(1);
    se->play();*/

    //this->startTimer(1000);
    this->setFixedSize(1280, 720);

    QPixmap pix_Background(":/img/bg_city.jpg");
    pix_Background = pix_Background.scaled(1280, 720);
    QPalette palette;
    palette.setBrush(QPalette::Background, pix_Background);
    this->setPalette(palette);





    displayWidgets();

    connect(pNewGame, &QPushButton::clicked, [=]()
    {
        setViewMode(false);
    });
    connect(pLoadSave, &QPushButton::clicked, [=]()
    {
        setViewMode(true);
        updateSaves();
    });
    connect(pRandSeed, &QPushButton::clicked, [=]()
    {
        sbSeed->setValue(qrand() % 10001);
    });

    connect(pPlay, &QPushButton::clicked, this, &Launcher::play);
    connect(pSetGameFile, &QPushButton::clicked, this, &Launcher::setGameFile);
    connect(pSetSaveFolder, &QPushButton::clicked, this, &Launcher::setSaveFolder);

    setViewMode(false);
}

Launcher::~Launcher()
{

}

void Launcher::displayWidgets()
{
    lTitle = new QLabel(QString(tr("City Builder")),this);
    lTitle->setGeometry(50,0,600,100);
    lTitle->setStyleSheet("QLabel { color : white; }");
    lTitle->setFont(QFont(QString("Segoe UI Semilight"),40));

    lMapSize = new QLabel("Map Size :", this);
    lMapSize->setGeometry(950, 100, 200, 50);
    lMapSize->setStyleSheet("QLabel { color : white; }");
    lMapSize->setFont(QFont(QString("Segoe UI Semilight"),20));

    lSeed = new QLabel("Seed :", this);
    lSeed->setGeometry(950, 150, 200, 50);
    lSeed->setStyleSheet("QLabel { color : white; }");
    lSeed->setFont(QFont(QString("Segoe UI Semilight"),20));

    pRandSeed = new QPushButton("", this);
    pRandSeed->setGeometry(1100, 150, 40, 40);
    pRandSeed->setIcon(QIcon(":/img/rand.png")); //source : https://d30y9cdsu7xlg0.cloudfront.net/png/45441-200.png via Google Images

    lDifficulty = new QLabel("Difficulty :", this);
    lDifficulty->setGeometry(950, 200, 200, 50);
    lDifficulty->setStyleSheet("QLabel { color : white; }");
    lDifficulty->setFont(QFont(QString("Segoe UI Semilight"),20));

    sbMapSize = new QSpinBox(this);
    sbMapSize->setGeometry(1100, 100, 150, 40);
    sbMapSize->setMinimum(16);
    sbMapSize->setMaximum(256);

    sbSeed = new QSpinBox(this);
    sbSeed->setGeometry(1150, 150, 100, 40);
    sbSeed->setMinimum(0);
    sbSeed->setMaximum(99999);
    sbSeed->setStyleSheet("QSpinBox { size : 14; }");

    cbDifficulty = new QComboBox(this);
    cbDifficulty->setGeometry(1100, 200, 150, 40);



    pSetGameFile = new QPushButton(tr("Set game file"), this);
    pSetGameFile->setGeometry(1100,20,150,30);
    pSetGameFile->setFont(QFont(QString("Segoe UI Semilight"),8));

    pSetSaveFolder = new QPushButton(tr("Set save folder"), this);
    pSetSaveFolder->setGeometry(1100,50,150,30);
    pSetSaveFolder->setFont(QFont(QString("Segoe UI Semilight"),8));

    lGameFile = new QLabel("", this);
    lGameFile->setGeometry(500,0,200,30);
    lGameFile->setFont(QFont(QString("Segoe UI Semilight"),8));

    lSaveFolder = new QLabel("", this);
    lSaveFolder->setGeometry(500,30,200,30);
    lSaveFolder->setFont(QFont(QString("Segoe UI Semilight"),8));


    //Liste des sauvegardes
    listSaves = new QListWidget(this);
    listSaves->setGeometry(950, 100, 300, 420);

    //Boutons
    pPlay = new QPushButton(QString(tr("Play")), this);
    pPlay->setGeometry(950,600,300,100);
    pPlay->setFont(QFont(QString("Segoe UI Semilight"),40));

    pNewGame = new QPushButton(QString(tr("New")),this);
    pNewGame->setGeometry(950,550,150,50);
    pNewGame->setFont(QFont(QString("Segoe UI Semilight"),20));

    pLoadSave = new QPushButton(QString(tr("Load")),this);
    pLoadSave->setGeometry(1100,550,150,50);
    pLoadSave->setFont(QFont(QString("Segoe UI Semilight"),20));
}

void Launcher::timerEvent(QTimerEvent *event)
{
    updateSaves();
    updateInfos();
}

void Launcher::updateInfos()
{
    QStringList sl = getSavesList();

    lGameFile->setText(QString(tr("Game path : %0")).arg(sPathGameFile));
    lSaveFolder->setText(QString(tr("Save path [%0] : %1").arg(sl.count()).arg(sPathSaveFolder)));

    if(QFileInfo(sPathGameFile).exists())
        lGameFile->setStyleSheet("color:green;");
    else
        lGameFile->setStyleSheet("color:red;");

    if(QDir(sPathSaveFolder).exists() && sl.count() > 0)
        lSaveFolder->setStyleSheet("color:green;");
    else
        lSaveFolder->setStyleSheet("color:red;");
}

void Launcher::updatePreview()
{

}

void Launcher::setGameFile()
{
    sPathGameFile.clear();
    sPathGameFile.append(QFileDialog::getOpenFileName(this, tr("Select the game file"), QString("C:/"), QString("CityBuilder.exe")));
    updateInfos();
}

void Launcher::setSaveFolder()
{
    sPathSaveFolder.clear();
    sPathSaveFolder.append(QFileDialog::getExistingDirectory(this, tr("Select the save folder"), QString("C:/"),QFileDialog::ShowDirsOnly));
    updateSaves();
    updateInfos();
}

void Launcher::updateSaves()
{
    updateListWidget(getSavesList());
}

QStringList Launcher::getSavesList()
{
    QDir dir(sPathSaveFolder);
    QStringList filter;
    filter << "*.savecb";
    return dir.entryList(filter, QDir::Files, QDir::Time);
}

void Launcher::updateListWidget(QStringList sl)
{
    int irow = listSaves->currentRow();
    listSaves->clear();
    for(int i = 0; i < sl.count(); i++)
        listSaves->addItem(sl.at(i));
    listSaves->setCurrentRow(irow);
}

void Launcher::setViewMode(bool b)
{
    listSaves->setVisible(b);
    lMapSize->setVisible(!b);
    lSeed->setVisible(!b);
    lDifficulty->setVisible(!b);
    sbMapSize->setVisible(!b);
    sbSeed->setVisible(!b);
    cbDifficulty->setVisible(!b);
    if(b)
    {
        pNewGame->setStyleSheet("QPushButton { background-color : grey; }");
        pLoadSave->setStyleSheet("QPushButton { background-color : white; }");
    }
    else
    {
        pNewGame->setStyleSheet("QPushButton { background-color : white; }");
        pLoadSave->setStyleSheet("QPushButton { background-color : grey; }");
    }
}

void Launcher::play()
{
    QString game = sPathGameFile;
    QFileInfo infogame(game);

    if(infogame.exists())
    {
        game.append(" ");
        if(listSaves->currentItem() != nullptr)
            game.append("-l " + listSaves->currentItem()->text());
        else
            game.append("-n");

        process.close();
        qDebug() << "starting :" << game;
        process.start(game);
    }
    else
        QMessageBox::information(this, "Error", QString(tr("File %0 is missing !\nTry to set the game directory.")).arg(game));
}
