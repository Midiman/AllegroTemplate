#ifndef _SECTIONMANAGER_HPP_
#define _SECTIONMANAGER_HPP_

union ALLEGRO_EVENT;

class Game;

class Section
{
    public:
        Section(Game *game);
        ~Section();

        virtual void Update();
        virtual void Draw();
        virtual void Event(ALLEGRO_EVENT event);
        virtual void SwitchIn();
        virtual void SwitchOut();

    protected:
        Game *fGame;
};

class SectionManager
{
    public:
        SectionManager();
        ~SectionManager();

        void Add(std::string name, Section *section);
        void Change(std::string name);

        void Update();
        void Draw();
        void Event(ALLEGRO_EVENT event);

    private:
        std::map<std::string, Section *> fSections;
        Section *fCurrentSection;
};

#endif // _SECTIONMANAGER_HPP_
