#include "Launcher.h"

Launcher::Launcher(QWidget *parent) : QMainWindow(parent)
{


    double r = 16.0/9.0;
    double width = 900;
    resize((int)width,(int)(width/r));


    vboxlayout = new QVBoxLayout(this);

    l_title = new QLabel(QString(tr("CITY BUILDER")),this);
    l_title->setGeometry(0,0, width, 100);
    l_title->setFont(QFont(QString("Arial"),40));

    p_newgame = new QPushButton(QString(tr("New")),this);
    p_newgame->setGeometry(0,100,300,100);
    p_newgame->setFont(QFont(QString("Arial"),20));

    p_loadsave = new QPushButton(QString(tr("Load")),this);
    p_loadsave->setGeometry(0,200,300,100);
    p_loadsave->setFont(QFont(QString("Arial"),20));

    p_setgamedirectory = new QPushButton(QString(tr("Set game directory")), this);
    p_setgamedirectory->setGeometry(700,0,200,30);
    p_setgamedirectory->setFont(QFont(QString("Arial"),8));

    l_saves = new QListWidget(this);
    l_saves->setGeometry(300, 100, 300, 300);

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
    });

    connect(p_setgamedirectory, &QPushButton::clicked, this, &Launcher::setGameDirectory);
}

void Launcher::setGameDirectory()
{
    s_gamedirectory = QFileDialog::getExistingDirectory(this, tr("Select the game folder"), QString(),QFileDialog::ShowDirsOnly);
    qDebug() << s_gamedirectory;
    loadSaves();
}

void Launcher::loadSaves()
{
    QDir dir(s_gamedirectory);
    QStringList filter;
    filter << "*.savecb";
    QStringList sl = dir.entryList(filter, QDir::Files, QDir::Time);

    for(int i = 0; i < sl.count(); i++)
    {
        l_saves->addItem(sl.at(i));
    }
}

void Launcher::setViewMode(bool b)
{
    l_saves->setVisible(b);
}

Launcher::~Launcher()
{

}
