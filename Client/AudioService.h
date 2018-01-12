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
 * Workpackage manager : Margueron Raphael
 * Description         :
 * This class handle the audio part -> the audio mixer. It can be access as a
 * singleton object. There is an enume that handle the sfx array
 * ***********************************************************************************
 */
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
