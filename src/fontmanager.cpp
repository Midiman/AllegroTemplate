#include <dirent.h>
#include <iostream>
#include <map>
#include <sys/stat.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "fontmanager.hpp"

Font::Font(std::string filename)
{
    for (int i = 6; i < 180; i++)
    {
        fFonts[i] = al_load_ttf_font(filename.c_str(), i, 0);
    }
}

Font::~Font()
{
    for (int i = 6; i < 180; i++)
    {
        ALLEGRO_FONT *font = fFonts[i];
        if (font != NULL)
        {
            al_destroy_font(font);
        }
    }
}

ALLEGRO_FONT *Font::GetFont(int size)
{
    return fFonts[size];
}

FontManager::FontManager()
{
    std::cout << "[FontManager] Initialising..." << std::endl;

    al_init_font_addon();

    fWorking = al_init_ttf_addon();

    if (!fWorking)
    {
        std::cout << "[FontManager] Warning: Failed to initialise corrrectly..." << std::endl;
    }
}

FontManager::~FontManager()
{
    std::cout << "[FontManager] Deinitialising..." << std::endl;

    std::map<std::string, Font *>::const_iterator it;
    for (it = fFonts.begin(); it != fFonts.end(); ++it)
    {
        delete it->second;
    }
}

void FontManager::DataWalk(std::string fullParent, std::string niceParent)
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

                            std::cout << "[FontManager] Loading texture: " << fullPath << " -> " << nicePath << std::endl;
                            fFonts[nicePath] = new Font(fullPath);
                        }
                    }
                }
            }
        }
    }
}

void FontManager::LoadFrom(std::string path)
{
    if (fWorking)
    {
        std::cout << "[FontManager] Loading fonts from: " << path << std::endl;

        DataWalk(path, "");
    }
}

Font *FontManager::Get(std::string name)
{
    return fFonts[name];
}

void FontManager::Draw(std::string name, int size, float x, float y, ALLEGRO_COLOR colour, std::string text, FontAlign align)
{
    Font *font = fFonts[name];
    if (font != NULL)
    {
        ALLEGRO_FONT *f = font->GetFont(size);
        if (f != NULL)
        {
            if (align == FontAlignLeft)
            {
                al_draw_text(f, colour, x, y, ALLEGRO_ALIGN_LEFT, text.c_str());
            }
            else if (align == FontAlignMiddle)
            {
                al_draw_text(f, colour, x, y, ALLEGRO_ALIGN_CENTRE, text.c_str());
            }
            else if (align == FontAlignRight)
            {
                al_draw_text(f, colour, x, y, ALLEGRO_ALIGN_RIGHT, text.c_str());
            }
        }
    }
}
