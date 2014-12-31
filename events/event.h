#ifndef  EVENT_H_INCLUDE
#define  EVENT_H_INCLUDE
#include <list>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <SDL2/SDL_events.h>
namespace PO
{
    class Event
    {

    public:
        unsigned int Order;
        SDL_Event Eve;

        ~Event() {}
        Event():Order(0) {}

        void operator= (const Event& Tec)       {Order=Tec.Order;Eve=Tec.Eve;}
        void operator= (const SDL_Event& P)     {Eve=P;}
        Event(const Event& P):Order(P.Order)    {Eve=P.Eve;}
        Event(const SDL_Event& K):Order(0),Eve(K){}

        /*Key Event*/
        bool IsKey()                    const   {return (IsKeyButtonDown()||IsKeyButtonUp());}
        bool IsKeyButtonDown()          const   {return Eve.type==SDL_KEYDOWN;}
        bool IsKeyButtonUp()            const   {return Eve.type==SDL_KEYUP;}

        //Window Event
        bool IsWindow()                 const   {return Eve.type==SDL_WINDOWEVENT;}
        bool IsWindowClose()            const   {return Eve.window.event==SDL_WINDOWEVENT_CLOSE;}
        bool GetWindowIndex(unsigned int & T) const;

        //Quit Event
        bool IsExit()                   const   {return Eve.type==SDL_QUIT;}

        //Text Event
        bool IsText()                   const   {return IsTextEditing()||IsTextInput();}
        bool IsTextEditing()            const   {return Eve.type==SDL_TEXTEDITING;}
        bool IsTextInput()              const   {return Eve.type==SDL_TEXTINPUT;}

        //Mouse Event
        bool IsMouse()                  const   {return IsMouseMotion()||IsMouseButtonDown()||IsMouseButtonUp();}
        bool IsMouseMotion()            const   {return Eve.type==SDL_MOUSEMOTION;}
        bool IsMouseButtonDown()        const   {return Eve.type==SDL_MOUSEBUTTONDOWN;}
        bool IsMouseButtonUp()          const   {return Eve.type==SDL_MOUSEBUTTONUP;}

        //Wheel Event
        bool IsWheel()                  const   {return Eve.type==SDL_MOUSEWHEEL;}

        class Key;
        class Mouse;
        class Wheel;
        class Text;
        class Window;
        enum EventStyle
        {
            KEY=0,
            MOUSE,
            WHEEL,
            TEXT,
            WINDOW,
            SYSTEM,
            MAX
        };
        //class

        /*class Mouse
        {

        };*/
    };

    struct EventBox
    {
        //enum EventStyle {Window=0,Mouse,Key,Wheel,Text,Max};
        void StoreEvent(const std::shared_ptr<Event>&);
        const std::vector<std::shared_ptr<Event>>& GetEventBox(Event::EventStyle L) const {return Box[L];}
        void Clear();
        const std::vector<std::shared_ptr<Event>>& GetWindowBox()   const   {return GetEventBox(Event::WINDOW);}
        const std::vector<std::shared_ptr<Event>>& GetMouseBox()    const   {return GetEventBox(Event::MOUSE);}
        const std::vector<std::shared_ptr<Event>>& GetKeyBox()      const   {return GetEventBox(Event::KEY);}
        const std::vector<std::shared_ptr<Event>>& GetTextBox()     const   {return GetEventBox(Event::TEXT);}
    private:
        std::vector<std::shared_ptr<Event>> Box[Max];
    };
}
#endif
