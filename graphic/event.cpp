#include "event.h"
namespace PO
{
    std::map<wchar_t,wchar_t> Event::LetterDefine;
    bool Event::MapInit=false;

    //void Event::

    bool Event::IsKeySeem()const
    {
        unsigned int Key(Eve.key.keysym.sym);
        if((Key>='!'&&Key<='~')||(Key>=SDLK_KP_1&&Key<=SDLK_KP_0)||Key==SDLK_SPACE)
            return true;
        return false;
    }

    wchar_t Event::GetKey() const
    {
        if(Eve.key.keysym.sym>='a'&&Eve.key.keysym.sym<='z')
        {
            bool Up=false;
            if(Eve.key.keysym.mod&KMOD_CAPS)
                Up=!Up;
            if(Eve.key.keysym.mod&KMOD_SHIFT)
                Up=!Up;
            if(Up)
                return Eve.key.keysym.sym-'a'+'A';
        }else if(Eve.key.keysym.sym>='!'&&Eve.key.keysym.sym<='~')
        {
            if(Eve.key.keysym.mod&KMOD_SHIFT)
            {
                if(!MapInit)
                {
                    LetterDefine['1']='!';
                    LetterDefine['2']='@';
                    LetterDefine['3']='#';
                    LetterDefine['4']='$';
                    LetterDefine['5']='%';
                    LetterDefine['6']='^';
                    LetterDefine['7']='&';
                    LetterDefine['8']='*';
                    LetterDefine['9']='(';
                    LetterDefine['0']=')';
                    LetterDefine['-']='_';
                    LetterDefine['=']='+';
                    LetterDefine['\\']='|';
                    LetterDefine['[']='{';
                    LetterDefine[']']='}';
                    LetterDefine[';']=':';
                    LetterDefine['\'']='"';
                    LetterDefine[',']='<';
                    LetterDefine['.']='>';
                    LetterDefine['/']='?';
                    MapInit=true;
                }
                return LetterDefine[Eve.key.keysym.sym];
            }
        }
        return Eve.key.keysym.sym;
    }

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
