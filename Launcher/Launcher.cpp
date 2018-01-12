#include "Launcher.h"

Launcher::Launcher(QWidget *parent) : QWidget(parent)
{
    //double r = 16.0/9.0;
    //double width = 900;
    //resize((int)width,(int)(width/r));

    this->startTimer(1000);
    QLabel *l = new QLabel(QString(tr("ALLO")),this);
    QPixmap pix_Background(":/img/bg_city.jpg");
    l->setGeometry(0,0, width(), height());
    l->setPixmap(pix_Background);


    l_title = new QLabel(QString(tr("CITY BUILDER")),this);
    l_title->setGeometry(0,0, width(), height());
    l_title->setFont(QFont(QString("Arial"),40));

    p_newgame = new QPushButton(QString(tr("New")),this);
    p_newgame->setGeometry(0,100,300,100);
    p_newgame->setFont(QFont(QString("Arial"),20));

    p_loadsave = new QPushButton(QString(tr("Load")),this);
    p_loadsave->setGeometry(0,200,300,100);
    p_loadsave->setFont(QFont(QString("Arial"),20));

    p_setGameFile = new QPushButton(tr("Set game file"), this);
    p_setGameFile->setGeometry(700,0,200,30);
    p_setGameFile->setFont(QFont(QString("Arial"),8));

    p_setSaveFolder = new QPushButton(tr("Set save folder"), this);
    p_setSaveFolder->setGeometry(700,30,200,30);
    p_setSaveFolder->setFont(QFont(QString("Arial"),8));

    l_gameFile = new QLabel("", this);
    l_gameFile->setGeometry(500,0,200,30);
    l_gameFile->setFont(QFont(QString("Arial"),8));

    l_saveFolder = new QLabel("", this);
    l_saveFolder->setGeometry(500,30,200,30);
    l_saveFolder->setFont(QFont(QString("Arial"),8));


    listWidgetSaves = new QListWidget(this);
    listWidgetSaves->setGeometry(300, 100, 300, 300);

    p_play = new QPushButton(QString(tr("Play")), this);
    p_play->setGeometry(600,100,300,100);
    p_play->setFont(QFont(QString("Arial"),20));

    connect(p_newgame, &QPushButton::clicked, [=]()
    {
        setViewMode(false);
    });
    connect(p_loadsave, &QPushButton::clicked, [=]()
    {
        setViewMode(true);
        updateSaves();
    });


    connect(p_play, &QPushButton::clicked, this, &Launcher::play);
    connect(p_setGameFile, &QPushButton::clicked, this, &Launcher::setGameFile);
    connect(p_setSaveFolder, &QPushButton::clicked, this, &Launcher::setSaveFolder);
}

Launcher::~Launcher()
{

}

void Launcher::timerEvent(QTimerEvent *event)
{
    updateSaves();
    updateInfos();
}

void Launcher::updateInfos()
{
    QStringList sl = getSavesList();

    l_gameFile->setText(QString(tr("Game path : %0")).arg(s_path_GameFile));
    l_saveFolder->setText(QString(tr("Save path [%0] : %1").arg(sl.count()).arg(s_path_SaveFolder)));

    if(QFileInfo(s_path_GameFile).exists())
        l_gameFile->setStyleSheet("color:green;");
    else
        l_gameFile->setStyleSheet("color:red;");

    if(QDir(s_path_SaveFolder).exists() && sl.count() > 0)
        l_saveFolder->setStyleSheet("color:green;");
    else
        l_saveFolder->setStyleSheet("color:red;");
}

void Launcher::setGameFile()
{
    s_path_GameFile.clear();
    s_path_GameFile.append(QFileDialog::getOpenFileName(this, tr("Select the game file"), QString("C:/"), QString("CityBuilder.exe")));
    updateInfos();
}

void Launcher::setSaveFolder()
{
    s_path_SaveFolder.clear();
    s_path_SaveFolder.append(QFileDialog::getExistingDirectory(this, tr("Select the save folder"), QString("C:/"),QFileDialog::ShowDirsOnly));
    updateSaves();
    updateInfos();
}

void Launcher::updateSaves()
{
    updateListWidget(getSavesList());
}

QStringList Launcher::getSavesList()
{
    QDir dir(s_path_SaveFolder);
    QStringList filter;
    filter << "*.savecb";
    return dir.entryList(filter, QDir::Files, QDir::Time);
}

void Launcher::updateListWidget(QStringList sl)
{
    int irow = listWidgetSaves->currentRow();
    listWidgetSaves->clear();
    for(int i = 0; i < sl.count(); i++)
        listWidgetSaves->addItem(sl.at(i));
    listWidgetSaves->setCurrentRow(irow);
}

void Launcher::setViewMode(bool b)
{
    listWidgetSaves->setVisible(b);
}

void Launcher::play()
{
    QString game = s_path_GameFile;
    QFileInfo infogame(game);

    if(infogame.exists())
    {
        game.append(" ");
        if(listWidgetSaves->currentItem() != nullptr)
            game.append("-l " + listWidgetSaves->currentItem()->text());
        else
            game.append("-n");

        process.close();
        qDebug() << "starting :" << game;
        process.start(game);
    }
    else
        QMessageBox::information(this, "Error", QString(tr("File %0 is missing !\nTry to set the game directory.")).arg(game));
}
