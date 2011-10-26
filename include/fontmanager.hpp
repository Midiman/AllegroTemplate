#ifndef _FONTMANAGER_HPP_
#define _FONTMANAGER_HPP_

struct ALLEGRO_FONT;
struct ALLEGRO_COLOR;

class FontManager
{
    public:
        enum FontAlign
        {
            FontAlignMiddle,
            FontAlignLeft,
            FontAlignRight
        };

        class Font
        {
            public:
                Font(std::string filename);
                ~Font();

                ALLEGRO_FONT *GetFont(int size);

            private:
                ALLEGRO_FONT *fFonts[200];
        };
        
        FontManager();
        ~FontManager();

        void DataWalk(std::string fullParent, std::string niceParent);
        void LoadFrom(std::string path);

        Font *Get(std::string name);
        void Draw(std::string name, int size, float x, float y, ALLEGRO_COLOR colour, std::string text, FontAlign align = FontAlignLeft);

    private:
        std::map<std::string, Font *> fFonts;
        bool fWorking;
};

#endif // _FONTMANAGER_HPP_
