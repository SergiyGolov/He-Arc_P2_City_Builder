#pragma once

class QSound;
class QSoundEffect;

class AudioService
{
public:
    static AudioService* getAudioService();
    void playMusic();
    void stopMusic();

    void setVolumeMaster(double);
    double getVolumeMaster();

    void setVolumeMusic(double);
    double getVolumeMusic();

    void setVolumeSfx(double);
    double getVolumeSfx();

    void playSfx(int s);
private:
    static AudioService* audioService;
    AudioService();

    void applyMaster();

    double volumeMaster;
    double volumeMusic;
    double volumeSfx;

    double logTolinear(double v);
    double linearTolog(double v);

    QSoundEffect* music;

    const int NBSFX = 7;
    QSoundEffect* sfx;
};

namespace sfxID
{
    enum
    {
        buildingAdded,
        buildingRemoved,
    };
}
