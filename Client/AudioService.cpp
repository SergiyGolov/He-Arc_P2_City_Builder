#include "AudioService.h"

#include <QAudio>
#include <QSound>
#include <QSoundEffect>
#include <QDebug>

AudioService* AudioService::audioService = nullptr;

AudioService* AudioService::getAudioService()
{
    if(audioService == nullptr)
        audioService = new AudioService();
    return audioService;
}

AudioService::AudioService()
{
    volumeMaster = 1.0;

    music = new QSoundEffect();
    music->setSource(QUrl::fromLocalFile(QString(":/audio/citybuilder_song.wav")));
    music->setLoopCount(QSoundEffect::Infinite);

    sfx = new QSoundEffect[NBSFX]();

    for(int i = 0; i < NBSFX; i++)
    {
        sfx[i].setLoopCount(1);
    }

    sfx[0].setSource(QUrl::fromLocalFile(QString(":/audio/truck.wav")));
    sfx[1].setSource(QUrl::fromLocalFile(QString(":/audio/hammerSFX1.wav")));
    sfx[2].setSource(QUrl::fromLocalFile(QString(":/audio/hammerSFX2.wav")));
    sfx[3].setSource(QUrl::fromLocalFile(QString(":/audio/hammerSFX3.wav")));
    sfx[4].setSource(QUrl::fromLocalFile(QString(":/audio/hammerSFX4.wav")));
    sfx[5].setSource(QUrl::fromLocalFile(QString(":/audio/hammerSFX5.wav")));
    sfx[6].setSource(QUrl::fromLocalFile(QString(":/audio/hammerSFX6.wav")));
}

void AudioService::playMusic()
{
    music->play();
}

void AudioService::stopMusic()
{
    music->stop();
}

void AudioService::playSfx(int s)
{
    int id;
    switch(s)
    {
        case sfxID::buildingAdded:
            id = rand() % 6 + 1;
        break;
        case sfxID::buildingRemoved:
            id = 0;
        break;
        default:
            id = 1;
        break;
    }
    sfx[id].play();
}

void AudioService::applyMaster()
{
    music->setVolume(logTolinear(volumeMusic * volumeMaster));
    for(int i = 0; i < NBSFX; i++)
    {
        sfx[i].setVolume(logTolinear(volumeSfx * volumeMaster));
    }
}


void AudioService::setVolumeMaster(double v)
{
    volumeMaster = v;
    applyMaster();
}

double AudioService::getVolumeMaster()
{
    return volumeMaster;
}

double AudioService::getVolumeMusic()
{
    return linearTolog(music->volume());
}

void AudioService::setVolumeMusic(double v)
{
    volumeMusic = v;
    applyMaster();
}

double AudioService::getVolumeSfx()
{
    return linearTolog(sfx[0].volume());
}

void AudioService::setVolumeSfx(double v)
{
    volumeSfx = v;
    applyMaster();
}

double AudioService::logTolinear(double v)
{
    return QAudio::convertVolume(v, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);
}

double AudioService::linearTolog(double v)
{
    return QAudio::convertVolume(v, QAudio::LinearVolumeScale, QAudio::LogarithmicVolumeScale);
}

