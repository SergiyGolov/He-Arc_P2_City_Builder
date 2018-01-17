#include "Launcher.h"
#include <QPixmap>
#include <QSoundEffect>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QGraphicsScene>
#include <QGraphicsView>


Launcher::Launcher(QMainWindow *parent) : QMainWindow(parent)
{
    this->setFixedSize(1280, 720);

    QPixmap pmBackground(":/img/bg_city.jpg");
    pmBackground = pmBackground.scaled(1280, 720);
    QPalette palette;
    palette.setBrush(QPalette::Background, pmBackground);
    this->setPalette(palette);

    displayWidgets();

    connect(pNewGame, &QPushButton::clicked, [=]()
    {
        pNewGame->setEnabled(false);
        pLoadSave->setEnabled(true);
        setViewMode(false);
        updateSaves();
        updateInfos();
    });
    connect(pLoadSave, &QPushButton::clicked, [=]()
    {
        pNewGame->setEnabled(true);
        pLoadSave->setEnabled(false);
        setViewMode(true);
        updateSaves();
        updateInfos();
    });
    connect(pRandSeed, &QPushButton::clicked, [=]()
    {
        sbSeed->setValue(qrand() % 10001);
    });
    connect(leGameName, &QLineEdit::textChanged, [=]()
    {
       if(leGameName->text()!="")
       {
           pPlay->setEnabled(true);
       }
    });
    connect(listSaves, &QListWidget::currentItemChanged, [=]()
    {
        if(listSaves->currentItem() != nullptr)
            pPlay->setEnabled(true);
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
    lGameFile = new QLabel("", this);
    lGameFile->setGeometry(470,20,800,30);
    lGameFile->setFont(QFont(QString("Segoe UI Semilight"),8));
    lSaveFolder = new QLabel("", this);
    lSaveFolder->setGeometry(470,50,800,30);
    lSaveFolder->setFont(QFont(QString("Segoe UI Semilight"),8));
    pSetGameFile = new QPushButton(tr("Set game file"), this);
    pSetGameFile->setGeometry(320,20,150,30);
    pSetGameFile->setFont(QFont(QString("Segoe UI Semilight"),8));
    sPathGameFile = QDir::currentPath() + "/debug/citybuilder.exe";
    //sPathSaveFolder = QDir::currentPath() + "/citybuilder.exe";

    pSetSaveFolder = new QPushButton(tr("Set save folder"), this);
    pSetSaveFolder->setGeometry(320,50,150,30);
    pSetSaveFolder->setFont(QFont(QString("Segoe UI Semilight"),8));
    sPathSaveFolder = QDir::currentPath() + "/debug/saves"; ///TODO
    //sPathSaveFolder = QDir::currentPath() + "/saves";

    lTitle = new QLabel(QString(tr("City Builder")),this);
    lTitle->setGeometry(20,0,600,100);
    lTitle->setStyleSheet("QLabel { color : white; }");
    lTitle->setFont(QFont(QString("Segoe UI Semilight"),40));
    lMapSize = new QLabel("Map Size :", this);
    lMapSize->setGeometry(950, 100, 200, 50);
    lMapSize->setStyleSheet("QLabel { color : white; }");
    lMapSize->setFont(QFont(QString("Segoe UI Semilight"),18));
    lSeed = new QLabel("Seed :", this);
    lSeed->setGeometry(950, 150, 200, 50);
    lSeed->setStyleSheet("QLabel { color : white; }");
    lSeed->setFont(QFont(QString("Segoe UI Semilight"),18));
    lDifficulty = new QLabel("Difficulty :", this);
    lDifficulty->setGeometry(950, 200, 200, 50);
    lDifficulty->setStyleSheet("QLabel { color : white; }");
    lDifficulty->setFont(QFont(QString("Segoe UI Semilight"),18));
    lGameName = new QLabel("Game name :", this);
    lGameName->setGeometry(950, 250, 200, 50);
    lGameName->setStyleSheet("QLabel { color : white; }");
    lGameName->setFont(QFont(QString("Segoe UI Semilight"),18));

    sbMapSize = new QSpinBox(this);
    sbMapSize->setGeometry(1100, 100, 150, 40);
    sbMapSize->setMinimum(16);
    sbMapSize->setMaximum(256);
    pRandSeed = new QPushButton("", this);
    pRandSeed->setGeometry(1100, 150, 40, 40);
    pRandSeed->setIcon(QIcon(":/img/rand.png")); //source : https://d30y9cdsu7xlg0.cloudfront.net/png/45441-200.png via Google Images
    sbSeed = new QSpinBox(this);
    sbSeed->setGeometry(1150, 150, 100, 40);
    sbSeed->setMinimum(0);
    sbSeed->setMaximum(10000);
    sbSeed->setStyleSheet("QSpinBox { size : 14; }");
    cbDifficulty = new QComboBox(this);
    cbDifficulty->setGeometry(1100, 200, 150, 40);
    cbDifficulty->addItem(QIcon(":img/easy.png"), "Easy");
    cbDifficulty->addItem(QIcon(":img/medium.png"), "Medium");
    cbDifficulty->addItem(QIcon(":img/hard.png"), "Hard");
    leGameName = new QLineEdit(this);
    leGameName->setGeometry(1100, 250, 150, 40);




    //Liste des sauvegardes
    listSaves = new QListWidget(this);
    listSaves->setGeometry(950, 100, 300, 420);

    //Preview
    pmPreview.load(":img/easy.png");
    pmPreview = pmPreview.scaled(600, 600);
    QLabel* lbl = new QLabel("Preview",this);
    lbl->setPixmap(pmPreview);
    lbl->setGeometry(320, 100, 600, 600);


    //Boutons
    pPlay = new QPushButton(QString(tr("Play")), this);
    pPlay->setGeometry(950,600,300,100);
    pPlay->setFont(QFont(QString("Segoe UI Semilight"),40));
    pPlay->setEnabled(false);

    pNewGame = new QPushButton(QString(tr("New")),this);
    pNewGame->setGeometry(950,550,150,50);
    pNewGame->setFont(QFont(QString("Segoe UI Semilight"),20));
    pNewGame->setEnabled(false);

    pLoadSave = new QPushButton(QString(tr("Load")),this);
    pLoadSave->setGeometry(1100,550,150,50);
    pLoadSave->setFont(QFont(QString("Segoe UI Semilight"),20));

    updateInfos();
    updateSaves();
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

void Launcher::updateSaves()
{
    updateListWidget(getSavesList());
}

void Launcher::updatePreview() //TODO
{


}

void Launcher::setGameFile()
{
    sPathGameFile.clear();
    sPathGameFile.append(QFileDialog::getOpenFileName(this, tr("Select the game file"), QDir::currentPath(), QString("CityBuilder.exe")));
    updateSaves();
    updateInfos();
}

void Launcher::setSaveFolder()
{
    sPathSaveFolder.clear();
    sPathSaveFolder.append(QFileDialog::getExistingDirectory(this, tr("Select the save folder"), QDir::currentPath() ,QFileDialog::ShowDirsOnly));
    updateSaves();
    updateInfos();
}

QStringList Launcher::getSavesList()
{
    QDir dir(sPathSaveFolder);
    QStringList filter;
    filter << "*.cbsave";
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
        pLoadSave->setStyleSheet("QPushButton { background-color : white; color : black}");
    }
    else
    {
        pNewGame->setStyleSheet("QPushButton { background-color : white; color : black }");
        pLoadSave->setStyleSheet("QPushButton { background-color : grey; }");
    }
    pPlay->setEnabled(false);
    /*
    if(b && listSaves->currentItem() != nullptr)
    {
        pPlay->setEnabled(true);
    }
    if(!b && leGameName->text() != "")
    {
        pPlay->setEnabled(true);
    }*/
}

void Launcher::play()
{
    QString game = sPathGameFile;
    QFileInfo infogame(game);

    if(infogame.exists())
    {
        game.append(" ");


        if(!pNewGame->isEnabled())
        {
            game.append("size=");
            game.append(QString::number(sbMapSize->value()));
            game.append(" difficulty=");
            game.append(cbDifficulty->currentText());
            game.append(" seed=");
            game.append(QString::number(sbSeed->value()));
            game.append(" name=");
            game.append(leGameName->text());
        }
        else
        {
            if(listSaves->currentItem() != nullptr)
                game.append(listSaves->currentItem()->text());
        }
        process.close();
        qDebug() << "starting :" << game;
        process.start(game);
    }
    else
        QMessageBox::information(this, "Error", QString(tr("File %0 is missing !\nTry to set the game directory.")).arg(game));
}
