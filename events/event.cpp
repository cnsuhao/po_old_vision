#include "event.h"
namespace PO
{


    bool Event::GetWindowIndex(unsigned int & T) const
    {
        switch(Eve.type)
        {
        case SDL_MOUSEMOTION:
            T=Eve.motion.windowID;
            return true;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            T=Eve.button.windowID;
            return true;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            T=Eve.key.windowID;
            return true;
        case SDL_MOUSEWHEEL:
            T=Eve.wheel.windowID;
            return true;
        case SDL_TEXTEDITING:
            T=Eve.edit.windowID;
            return true;
        case SDL_TEXTINPUT:
            T=Eve.text.windowID;
            return true;
        case SDL_WINDOWEVENT:
            T=Eve.window.windowID;
            return true;
        default:
            return false;
        }
    }
    void EventBox::Clear()
    {
        Box[Mouse].clear();
        Box[Key].clear();
        Box[Window].clear();
        Box[Wheel].clear();
        Box[Text].clear();
    }
    void EventBox::StoreEvent(const std::shared_ptr<Event>& I)
    {
        if(I->IsMouse())
            Box[Mouse].push_back(I);
        else if(I->IsKey())
            Box[Key].push_back(I);
        else if(I->IsWindow())
            Box[Window].push_back(I);
        else if(I->IsWheel())
            Box[Wheel].push_back(I);
        else if(I->IsText())
            Box[Text].push_back(I);
    }
}
