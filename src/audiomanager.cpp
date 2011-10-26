#include <cstring>
#include <dirent.h>
#include <iostream>
#include <map>
#include <sys/stat.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "audiomanager.hpp"

Sound::Sound(std::string filename)
{
    fSample = al_load_sample(filename.c_str());
}

Sound::~Sound()
{
    if (fSample != NULL)
    {
        al_destroy_sample(fSample);
    }
}

ALLEGRO_SAMPLE *Sound::GetSample()
{
    return fSample;
}

Music::Music(std::string filename)
{
    fStream = al_load_audio_stream(filename.c_str(), 4, 2048);
}

Music::~Music()
{
    if (fStream != NULL)
    {
        al_destroy_audio_stream(fStream);
    }
}

ALLEGRO_AUDIO_STREAM *Music::GetStream()
{
    return fStream;
}

AudioManager::AudioManager()
{
    std::cout << "[AudioManager] Initialising..." << std::endl;

    fWorking = true;

    fSoundVolume = 1.0;
    fSoundEnabled = true;

    fMusicEnabled = true;

    if (!al_install_audio())
        fWorking = false;

    if (fWorking && !al_init_acodec_addon())
        fWorking = false;

    if (fWorking && !al_reserve_samples(1))
        fWorking = false;

    if (fWorking)
    {
        fMusicVoice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
        if (fMusicVoice == NULL)
        {
            fWorking = false;
        }
        else
        {
            fMusicMixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);

            if (fMusicMixer == NULL)
            {
                fWorking = false;
            }
            else
            {
                al_attach_mixer_to_voice(fMusicMixer, fMusicVoice);
            }
        }
    }

    if (!fWorking)
    {
        std::cout << "[AudioManager] Warning: Failed to initialise corrrectly..." << std::endl;
    }
}

AudioManager::~AudioManager()
{
    std::cout << "[AudioManager] Deinitialising..." << std::endl;

    std::map<std::string, Sound *>::const_iterator it;
    for (it = fSounds.begin(); it != fSounds.end(); ++it)
    {
        delete it->second;
    }

    std::map<std::string, Music *>::const_iterator it2;
    for (it2 = fMusic.begin(); it2 != fMusic.end(); ++it2)
    {
        delete it2->second;
    }

    if (fWorking)
    {
        al_destroy_mixer(fMusicMixer);
        al_destroy_voice(fMusicVoice);
    }
}

void AudioManager::DataWalkSounds(std::string fullParent, std::string niceParent)
{
    DIR *dp = opendir(fullParent.c_str());
    if (dp != NULL)
    {
        struct dirent *ep;
        while ((ep = readdir(dp)) != NULL)
        {
            if (strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0)
            {
                std::string fullPath = fullParent + std::string("/") + ep->d_name;
                
                struct stat s;
                if (stat(fullPath.c_str(), &s) == 0)
                {
                    if (S_ISDIR(s.st_mode))
                    {
                        std::string nicePath = niceParent + ep->d_name + std::string("/");

                        DataWalkSounds(fullPath, nicePath);
                    }
                    else
                    {
                        if (std::string(ep->d_name).find_last_of(".") != std::string::npos)
                        {
                            std::string name = std::string(ep->d_name).substr(0, std::string(ep->d_name).find_last_of("."));
                            std::string nicePath = niceParent + name;

                            std::cout << "[AudioManager] Loading sound: " << fullPath << " -> " << nicePath << std::endl;
                            fSounds[nicePath] = new Sound(fullPath);
                        }
                    }
                }
            }
        }
    }
}

void AudioManager::LoadSoundsFrom(std::string path)
{
    if (fWorking)
    {
        std::cout << "[AudioManager] Loading sounds from: " << path << std::endl;

        DataWalkSounds(path, "");
    }
}

Sound *AudioManager::GetSound(std::string name)
{
    return fSounds[name];
}

void AudioManager::PlaySound(std::string name)
{
    if (fSoundEnabled && fWorking)
    {
        Sound *sound = fSounds[name];
        if (sound != NULL)
        {
            ALLEGRO_SAMPLE *sample = sound->GetSample();
            if (sample != NULL)
            {
                al_play_sample(sample, fSoundVolume, ALLEGRO_AUDIO_PAN_NONE, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
        }
    }
}

void AudioManager::SetSoundVolume(float v)
{
    fSoundVolume = v;
}

void AudioManager::SetSoundEnabled(bool e)
{
    fSoundEnabled = e;
}

float AudioManager::GetSoundVolume()
{
    return fSoundVolume;
}

bool AudioManager::GetSoundEnabled()
{
    return fSoundEnabled;
}

void AudioManager::DataWalkMusic(std::string fullParent, std::string niceParent)
{
    DIR *dp = opendir(fullParent.c_str());
    if (dp != NULL)
    {
        struct dirent *ep;
        while ((ep = readdir(dp)) != NULL)
        {
            if (strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0)
            {
                std::string fullPath = fullParent + std::string("/") + ep->d_name;
                
                struct stat s;
                if (stat(fullPath.c_str(), &s) == 0)
                {
                    if (S_ISDIR(s.st_mode))
                    {
                        std::string nicePath = niceParent + ep->d_name + std::string("/");

                        DataWalkMusic(fullPath, nicePath);
                    }
                    else
                    {
                        if (std::string(ep->d_name).find_last_of(".") != std::string::npos)
                        {
                            std::string name = std::string(ep->d_name).substr(0, std::string(ep->d_name).find_last_of("."));
                            std::string nicePath = niceParent + name;

                            std::cout << "[AudioManager] Loading music: " << fullPath << " -> " << nicePath << std::endl;
                            fMusic[nicePath] = new Music(fullPath);
                        }
                    }
                }
            }
        }
    }
}

void AudioManager::LoadMusicFrom(std::string path)
{
    if (fWorking)
    {
        std::cout << "[AudioManager] Loading music from: " << path << std::endl;

        DataWalkMusic(path, "");
    }
}

Music *AudioManager::GetMusic(std::string name)
{
    return fMusic[name];
}

void AudioManager::PlayMusic(std::string name)
{
    if (fMusicEnabled && fWorking)
    {
        Music *music = fMusic[name];
        if (music != NULL)
        {
            ALLEGRO_AUDIO_STREAM *stream = music->GetStream();
            if (stream != NULL)
            {
                al_attach_audio_stream_to_mixer(stream, fMusicMixer);
            }
        }
    }
}

void AudioManager::SetMusicVolume(float v)
{
    if (fWorking)
    {
        // Re-enable when 5.1 is released
        //al_set_mixer_gain(fMusicMixer, v);
    }
}

void AudioManager::SetMusicEnabled(bool e)
{
    fMusicEnabled = e;
}

float AudioManager::GetMusicVolume()
{
    if (fWorking)
    {
        // Re-enable when 5.1 is released
        //return al_get_mixer_gain(fMusicMixer);
        return 1.0;
    }

    return 1.0;
}

bool AudioManager::GetMusicEnabled()
{
    return fMusicEnabled;
}

bool AudioManager::GetWorking()
{
    return fWorking;
}
