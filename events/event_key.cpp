#include "event_key.h"
namespace PO
{
    std::map<wchar_t,wchar_t> Event::Key::LetterDefine;
    bool Event::Key::MapInit=false;

    //void Event::

    bool Event::Key::IsVisible()const
    {
        unsigned int Key(Eve.key.keysym.sym);
        if((Key>='!'&&Key<='~')||(Key>=SDLK_KP_1&&Key<=SDLK_KP_0)||Key==SDLK_SPACE)
            return true;
        return false;
    }

    wchar_t Event::Key::Get() const
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
}
