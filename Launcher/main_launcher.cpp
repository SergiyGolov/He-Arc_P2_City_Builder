#include "Launcher.h"
#include <QApplication>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QMediaPlaylist>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Launcher w;

    QMediaPlaylist* playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl::fromLocalFile(QString("CityBuilderLauncher.mp3")));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    QMediaPlayer* music = new QMediaPlayer();
    music->setPlaylist(playlist);
    music->setVolume(100);
    music->play();

    w.show();

    //AudioService::getAudioService()->playMusic();
    return a.exec();
}
