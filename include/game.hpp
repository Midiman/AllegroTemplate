#ifndef _GAME_HPP_
#define _GAME_HPP_

struct ALLEGRO_DISPLAY;
struct ALLEGRO_EVENT_QUEUE;
struct ALLEGRO_TIMER;

class SectionManager;
class TextureManager;
class FontManager;
class AudioManager;

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
        
        int GetWidth();
        int GetHeight();
        int GetHalfWidth();
        int GetHalfHeight();

        FontManager *GetFontManager();
        TextureManager *GetTextureManager();
        AudioManager *GetAudioManager();

    private:
        int fWidth, fHeight;
        bool fRunning, fRedraw;
        ALLEGRO_DISPLAY *fDisplay;
        ALLEGRO_EVENT_QUEUE *fEventQueue;
        ALLEGRO_TIMER *fTimer;

        SectionManager *fSectionManager;
        TextureManager *fTextureManager;
        FontManager *fFontManager;
        AudioManager *fAudioManager;
};

#endif // _GAME_HPP_
