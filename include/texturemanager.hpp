#ifndef _TEXTUREMANAGER_HPP_
#define _TEXTUREMANAGER_HPP_

struct ALLEGRO_BITMAP;

class Texture
{
    public:
        Texture(std::string filename);
        ~Texture();

        ALLEGRO_BITMAP *GetBitmap();

    private:
        ALLEGRO_BITMAP *fBitmap;
};

class TextureManager
{
    public:
        TextureManager();
        ~TextureManager();

        void DataWalk(std::string fullParent, std::string niceParent);
        void LoadFrom(std::string path);

        Texture *Get(std::string name);
        void Draw(std::string name, float x, float y);

    private:
        std::map<std::string, Texture *> fTextures;
};

#endif // _TEXTUREMANAGER_HPP_
