#ifndef EVENT_MOUSE_H_INCLUDED
#define EVENT_MOUSE_H_INCLUDED
#include "event.h"
namespace PO
{
    class Event::Mouse
    {
        const SDL_Event& Eve;
        void* operator new( size_t ) =delete ;
    public:
        Mouse(const Event& E):Eve(E.Eve) {}
        Mouse(const std::shared_ptr<Event>& E):Eve(E->Eve) {}
        operator bool ()                const   {return IsMotion()||IsButtonDown()||IsButtonUp();}
        bool IsMotion()                 const   {return Eve.type==SDL_MOUSEMOTION;}
        bool IsButtonDown()             const   {return Eve.type==SDL_MOUSEBUTTONDOWN;}
        bool IsButtonUp()               const   {return Eve.type==SDL_MOUSEBUTTONUP;}
        bool IsLeft()                   const   {return Eve.button.button==SDL_BUTTON_LEFT;}
        bool IsMiddle()                 const   {return Eve.button.button==SDL_BUTTON_MIDDLE;}
        bool IsRight()                  const   {return Eve.button.button==SDL_BUTTON_RIGHT;}
        int LocationX()                 const   {if(IsMotion()) return Eve.motion.x;return Eve.button.x;}
        int LocationY()                 const   {if(IsMotion()) return Eve.motion.y;return Eve.button.y;}
        int MotionX()                   const   {return Eve.motion.xrel;}
        int MotionY()                   const   {return Eve.motion.yrel;}
        void GetLocation(int &K,int &P) const   {K=LocationX();P=LocationY();}
        void GetMotion(int &K,int& P)   const   {K=MotionX();P=MotionY();}
    };

    class Event::Wheel
    {
        const SDL_Event& Eve;
        void* operator new( size_t ) =delete ;
    public:
        Wheel(const Event& E):Eve(E.Eve) {}
        Wheel(const std::shared_ptr<Event>& E):Eve(E->Eve) {}
        operator bool ()                const   {return Eve.type==SDL_MOUSEWHEEL;}
        void Get(int& X,int& Y)    const   {X=Eve.wheel.x;Y=Eve.wheel.y;}
        int GetVertically()        const   {return Eve.wheel.y;}
        int GetHorizontally()      const   {return Eve.wheel.x;}
    };
}


#endif // EVENT_MOUSE_H_INCLUDED
