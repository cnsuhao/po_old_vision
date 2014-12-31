#ifndef EVENT_KEY_H_INCLUDED
#define EVENT_KEY_H_INCLUDED
#include "event.h"
namespace PO
{
    class Event::Key
    {
        const SDL_Event& Eve;
        void* operator new( size_t ) =delete ;
        static std::map<wchar_t,wchar_t> LetterDefine;
        static bool MapInit;
    public:
        Key(const Event& E):Eve(E.Eve) {}
        Key(const std::shared_ptr<Event>& E):Eve(E->Eve) {}
        operator bool ()            const   {return IsButtonDown()||IsButtonUp(); }
        bool Is(wchar_t I)              const   {return Get()==I;}
        bool IsDefine(unsigned int I)   const   {return Eve.key.keysym.sym==I;}
        unsigned int GetDefine()        const   {return Eve.key.keysym.sym;}
        wchar_t GetReal()               const   {return Eve.key.keysym.sym;}
        wchar_t Get()                   const;
        char GetChar()                  const   {return static_cast<char>(Get());}
        bool IsVisible()                const;
        bool IsEsc()                    const   {return Eve.key.keysym.sym==SDLK_ESCAPE;}
        bool IsUp()                     const   {return Eve.key.keysym.sym==SDLK_UP;}
        bool IsDown()                   const   {return Eve.key.keysym.sym==SDLK_DOWN;}
        bool IsRight()                  const   {return Eve.key.keysym.sym==SDLK_RIGHT;}
        bool IsLeft()                   const   {return Eve.key.keysym.sym==SDLK_LEFT;}
        bool IsDelete()                 const   {return Eve.key.keysym.sym==SDLK_BACKSPACE ;}
        bool IsEnter()                  const   {return Eve.key.keysym.sym==SDLK_RETURN;}
        bool IsSpace()                  const   {return Eve.key.keysym.sym==SDLK_SPACE;}
        bool IsModeShift()              const   {return Eve.key.keysym.mod&KMOD_SHIFT;}
        bool IsModeCtrl()               const   {return Eve.key.keysym.mod&KMOD_CTRL;}
        bool IsModeAlt()                const   {return Eve.key.keysym.mod&KMOD_ALT;}
        bool IsButtonDown()             const   {return Eve.type==SDL_KEYDOWN;}
        bool IsButtonUp()               const   {return Eve.type==SDL_KEYUP;}
    };

    class Event::Text
    {
        const SDL_Event& Eve;
        void* operator new( size_t ) =delete ;
    public:
        Text(const Event& E):Eve(E.Eve) {}
        Text(const std::shared_ptr<Event>& E):Eve(E->Eve) {}
        operator bool ()                const   {return IsEditing()||IsInput();}
        bool IsEditing()                const   {return Eve.type==SDL_TEXTEDITING;}
        bool IsInput()                  const   {return Eve.type==SDL_TEXTINPUT;}
        const char* GetInput()          const   {return Eve.text.text;}
        const char* GetEidt()           const   {return Eve.edit.text;}
        unsigned int GetEditStart()     const   {return Eve.edit.start;}
        unsigned int GetEditLength()    const   {return Eve.edit.length;}
    };
}


#endif // EVENT_KEY_H_INCLUDED
