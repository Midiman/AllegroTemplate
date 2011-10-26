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

AudioManager::AudioManager()
{
    std::cout << "[AudioManager] Initialising..." << std::endl;

    fWorking = true;

    fSoundVolume = 1.0;
    fSoundEnabled = true;

    if (!al_install_audio())
        fWorking = false;

    if (fWorking && !al_init_acodec_addon())
        fWorking = false;

    if (fWorking && !al_reserve_samples(1))
        fWorking = false;

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
    if (fSoundEnabled)
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
