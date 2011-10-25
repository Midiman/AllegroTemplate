#include <cstring>
#include <dirent.h>
#include <iostream>
#include <map>
#include <sys/stat.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "texturemanager.hpp"

Texture::Texture(std::string filename)
{
    fBitmap = al_load_bitmap(filename.c_str());
}

Texture::~Texture()
{
    if (fBitmap != NULL)
    {
        al_destroy_bitmap(fBitmap);
    }
}

ALLEGRO_BITMAP *Texture::GetBitmap()
{
    return fBitmap;
}

TextureManager::TextureManager()
{
    std::cout << "[TextureManager] Initialising..." << std::endl;

    al_init_image_addon();
}

TextureManager::~TextureManager()
{
    std::cout << "[TextureManager] Deinitialising..." << std::endl;

    std::map<std::string, Texture *>::const_iterator it;
    for (it = fTextures.begin(); it != fTextures.end(); ++it)
    {
        delete it->second;
    }
}

void TextureManager::DataWalk(std::string fullParent, std::string niceParent)
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

                        DataWalk(fullPath, nicePath);
                    }
                    else
                    {
                        if (std::string(ep->d_name).find_last_of(".") != std::string::npos)
                        {
                            std::string name = std::string(ep->d_name).substr(0, std::string(ep->d_name).find_last_of("."));
                            std::string nicePath = niceParent + name;

                            std::cout << "[TextureManager] Loading texture: " << fullPath << " -> " << nicePath << std::endl;
                            fTextures[nicePath] = new Texture(fullPath);
                        }
                    }
                }
            }
        }
    }
}

void TextureManager::LoadFrom(std::string path)
{
    std::cout << "[TextureManager] Loading textures from: " << path << std::endl;

    DataWalk(path, "");
}

Texture *TextureManager::Get(std::string name)
{
    return fTextures[name];
}

void TextureManager::Draw(std::string name, float x, float y)
{
    Texture *tex = fTextures[name];
    if (tex != NULL)
    {
        if (tex->GetBitmap() != NULL)
        {
            al_draw_bitmap(tex->GetBitmap(), x, y, 0);
        }
    }
}
