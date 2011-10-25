#include <iostream>
#include <map>

#include <allegro5/allegro.h>

#include "sectionmanager.hpp"

Section::Section(Game *game)
{
    fGame = game;
}

Section::~Section()
{
    
}


void Section::Update()
{
    
}

void Section::Draw()
{
    
}

void Section::Event(ALLEGRO_EVENT event)
{
    
}

void Section::SwitchIn()
{
    
}

void Section::SwitchOut()
{
    
}

SectionManager::SectionManager()
{
    std::cout << "[SectionManager] Initialising..." << std::endl;

    fCurrentSection = NULL;
}

SectionManager::~SectionManager()
{
    std::cout << "[SectionManager] Deinitialising..." << std::endl;

    std::map<std::string, Section *>::const_iterator it;
    for (it = fSections.begin(); it != fSections.end(); ++it)
    {
        delete it->second;
    }
}

void SectionManager::Add(std::string name, Section *section)
{
    std::cout << "[SectionManager] " << "Registering: " << name << std::endl;
    if (fSections[name] != NULL)
    {
        delete fSections[name];
        std::cout << "[SectionManager] Warning: previous version of this section has been replaced" << std::endl;
    }

    fSections[name] = section;
}

void SectionManager::Change(std::string name)
{
    std::cout << "[SectionManager] Changing section: " << name << std::endl;

    if (fCurrentSection != NULL)
    {
        fCurrentSection->SwitchOut();
    }

    fCurrentSection = fSections[name];

    if (fCurrentSection == NULL)
    {
        std::cout << "[SectionManager] Section doesn't exist" << std::endl;
    }

    if (fCurrentSection != NULL)
    {
        fCurrentSection->SwitchIn();
    }
}

void SectionManager::Update()
{
    if (fCurrentSection != NULL)
    {
        fCurrentSection->Update();
    }
}

void SectionManager::Draw()
{
    if (fCurrentSection != NULL)
    {
        fCurrentSection->Draw();
    }
}

void SectionManager::Event(ALLEGRO_EVENT event)
{
    if (fCurrentSection != NULL)
    {
        fCurrentSection->Event(event);
    }
}
