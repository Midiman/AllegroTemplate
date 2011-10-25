#ifndef _GAME_HPP_
#define _GAME_HPP_

struct ALLEGRO_DISPLAY;
struct ALLEGRO_EVENT_QUEUE;

class SectionManager;
class TextureManager;
class FontManager;

class Game
{
    public:
        Game(int argc, char *argv[]);
        ~Game();

        bool Init();
        bool InitAllegro();
        bool InitData();

        void Run();
        void CheckEvents();
        void Update();
        void Draw();

        void End();

    private:
        int fWidth, fHeight;
        bool fRunning;
        ALLEGRO_DISPLAY *fDisplay;
        ALLEGRO_EVENT_QUEUE *fEventQueue;

        SectionManager *fSectionManager;
        TextureManager *fTextureManager;
        FontManager *fFontManager;
};

#endif // _GAME_HPP_
