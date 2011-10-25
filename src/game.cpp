#include <iostream>
#include <map>

#include <allegro5/allegro.h>

#include "sectionmanager.hpp"
#include "texturemanager.hpp"
#include "fontmanager.hpp"
#include "game.hpp"

static void sAssertHandler(const char *expr, const char *file, int line, const char *func)
{
    std::cout << "Crashed :/" << std::endl;
    std::cout << " Expression: " << expr << std::endl;
    std::cout << " File: " << file << std::endl;
    std::cout << " Line: " << line << std::endl;
    std::cout << " Function: " << func << std::endl;

    exit(1);
}

Game::Game(int argc, char *argv[])
{
    fWidth = 800;
    fHeight = 600;

    fRunning = true;
}

Game::~Game()
{
}

bool Game::Init()
{
    if (!InitAllegro())
        return false;

    if (!InitData())
        return false;

    return true;
}

bool Game::InitAllegro()
{
    std::cout << "[Allegro] Initialising..." << std::endl;

    al_register_assert_handler(sAssertHandler);

    if (!al_init())
    {
        std::cout << "Failed to initialise Allegro" << std::endl;
        return false;
    }

    al_set_app_name("Game");
    al_set_org_name("Organisation");

    al_set_new_display_flags(ALLEGRO_OPENGL);
    fDisplay = al_create_display(fWidth, fHeight);
    if (!fDisplay)
        return false;

    al_install_keyboard();
    al_install_mouse();

    fEventQueue = al_create_event_queue();
    al_register_event_source(fEventQueue, al_get_display_event_source(fDisplay));
    al_register_event_source(fEventQueue, al_get_keyboard_event_source());
    al_register_event_source(fEventQueue, al_get_mouse_event_source());

    return true;
}

bool Game::InitData()
{
    fTextureManager = new TextureManager();
    fTextureManager->LoadFrom("data/textures");

    fFontManager = new FontManager();
    fFontManager->LoadFrom("data/fonts");

    fSectionManager = new SectionManager();

    return true;
}

void Game::Run()
{
    while (fRunning)
    {
        CheckEvents();
        Update();
        Draw();

        al_flip_display();
    }
}

void Game::CheckEvents()
{
    ALLEGRO_EVENT event;
    while (al_get_next_event(fEventQueue, &event))
    {
        switch (event.type)
        {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                fRunning = false;
                break;
        }

        fSectionManager->Event(event);
    }
}

void Game::Update()
{
    fSectionManager->Update();
}

void Game::Draw()
{
    fSectionManager->Draw();
}

void Game::End()
{
    delete fSectionManager;
    delete fTextureManager;
    delete fFontManager;

    std::cout << "[Allegro] Deinitialising..." << std::endl;

    al_destroy_event_queue(fEventQueue);
    al_destroy_display(fDisplay);
}
