#ifndef _AUDIOMANAGER_HPP_
#define _AUDIOMANAGER_HPP_

struct ALLEGRO_SAMPLE;

class Sound
{
    public:
        Sound(std::string filename);
        ~Sound();

        ALLEGRO_SAMPLE *GetSample();

    private:
        ALLEGRO_SAMPLE *fSample;
};

class AudioManager
{
    public:
        AudioManager();
        ~AudioManager();

        void DataWalkSounds(std::string fullParent, std::string niceParent);
        void LoadSoundsFrom(std::string path);

        Sound *GetSound(std::string name);
        void PlaySound(std::string name);
        void SetSoundVolume(float v);
        void SetSoundEnabled(bool e);
        float GetSoundVolume();
        bool GetSoundEnabled();

    private:
        std::map<std::string, Sound *> fSounds;
        float fSoundVolume;
        bool fSoundEnabled;

        bool fWorking;
};

#endif // _AUDIOMANAGER_HPP_
