#ifndef _AUDIOMANAGER_HPP_
#define _AUDIOMANAGER_HPP_

struct ALLEGRO_SAMPLE;
struct ALLEGRO_AUDIO_STREAM;
struct ALLEGRO_VOICE;
struct ALLEGRO_MIXER;

class Sound
{
    public:
        Sound(std::string filename);
        ~Sound();

        ALLEGRO_SAMPLE *GetSample();

    private:
        ALLEGRO_SAMPLE *fSample;
};

class Music
{
    public:
        Music(std::string filename);
        ~Music();

        ALLEGRO_AUDIO_STREAM *GetStream();

    private:
        ALLEGRO_AUDIO_STREAM *fStream;
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

        void DataWalkMusic(std::string fullParent, std::string niceParent);
        void LoadMusicFrom(std::string path);

        Music *GetMusic(std::string name);
        void PlayMusic(std::string name);
        void SetMusicVolume(float v);
        void SetMusicEnabled(bool e);
        float GetMusicVolume();
        bool GetMusicEnabled();

        bool GetWorking();

    private:
        std::map<std::string, Sound *> fSounds;
        float fSoundVolume;
        bool fSoundEnabled;

        std::map<std::string, Music *> fMusic;
        bool fMusicEnabled;
        ALLEGRO_VOICE *fMusicVoice;
        ALLEGRO_MIXER *fMusicMixer;

        bool fWorking;
};

#endif // _AUDIOMANAGER_HPP_
