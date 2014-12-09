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
        static std::map<wchar_t,wchar_t> LetterDefine;
        static bool MapInit;
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
        bool IsKey()                    const   {return (IsKeyButtonDown()||IsKeyButtonUp())&&(!SDL_IsTextInputActive()||!IsKeySeem());}
        bool IsKey(wchar_t I)           const   {return GetKey()==I;}
        bool IsKeyDefine(int I)const   {return Eve.key.keysym.sym==I;}
        unsigned int GetKeyDefine()     const   {return Eve.key.keysym.sym;}
        wchar_t GetRealKey()            const   {return Eve.key.keysym.sym;}
        wchar_t GetKey()                const;
        char GetChar()                  const   {return static_cast<char>(GetKey());}
        bool IsKeySeem()                const;
        bool IsKeyEsc()                 const   {return Eve.key.keysym.sym==SDLK_ESCAPE;}
        bool IsKeyUp()                  const   {return Eve.key.keysym.sym==SDLK_UP;}
        bool IsKeyDown()                const   {return Eve.key.keysym.sym==SDLK_DOWN;}
        bool IsKeyRight()               const   {return Eve.key.keysym.sym==SDLK_RIGHT;}
        bool IsKeyLeft()                const   {return Eve.key.keysym.sym==SDLK_LEFT;}
        bool IsKeyDelete()              const   {return Eve.key.keysym.sym==SDLK_BACKSPACE ;}
        bool IsKeyEnter()               const   {return Eve.key.keysym.sym==SDLK_RETURN;}
        bool IsKeySpace()               const   {return Eve.key.keysym.sym==SDLK_SPACE;}
        bool IsKeyModeShift()           const   {return Eve.key.keysym.mod&KMOD_SHIFT;}
        bool IsKeyModeCtrl()            const   {return Eve.key.keysym.mod&KMOD_CTRL;}
        bool IsKeyModeAlt()             const   {return Eve.key.keysym.mod&KMOD_ALT;}
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
        const char* GetTextInput()      const   {return Eve.text.text;}
        const char* GetEidtInput()      const   {return Eve.edit.text;}
        unsigned int GetEditStart()     const   {return Eve.edit.start;}
        unsigned int GetEditLength()    const   {return Eve.edit.length;}
        bool TextInputShield(  const Event&  );
        static bool IsTextActive()              {return SDL_IsTextInputActive();}

        //Mouse Event
        bool IsMouse()                  const   {return IsMouseMotion()||IsMouseButtonDown()||IsMouseButtonUp();}
        bool IsMouseMotion()            const   {return Eve.type==SDL_MOUSEMOTION;}
        bool IsMouseButtonDown()        const   {return Eve.type==SDL_MOUSEBUTTONDOWN;}
        bool IsMouseButtonUp()          const   {return Eve.type==SDL_MOUSEBUTTONUP;}
        bool IsMouseButtonLeft()        const   {return Eve.button.button==SDL_BUTTON_LEFT;}
        bool IsMouseButtonMiddle()      const   {return Eve.button.button==SDL_BUTTON_MIDDLE;}
        bool IsMouseButtonRight()       const   {return Eve.button.button==SDL_BUTTON_RIGHT;}
        int MouseLocationX()            const   {if(IsMouseMotion()) return Eve.motion.x;return Eve.button.x;}
        int MouseLocationY()            const   {if(IsMouseMotion()) return Eve.motion.y;return Eve.button.y;}
        int MouseMotionX()              const   {return Eve.motion.xrel;}
        int MouseMotionY()              const   {return Eve.motion.yrel;}
        void GetMouse(int &K,int &P)    const   {K=MouseLocationX();P=MouseLocationY();}
        void GetMouseMotion(int &K,int& P)const {K=MouseMotionX();P=MouseMotionY();}

        //Wheel Event
        bool IsWheel()                  const   {return Eve.type==SDL_MOUSEWHEEL;}
        void GetWheel(int& X,int& Y)    const   {X=Eve.wheel.x;Y=Eve.wheel.y;}
        int GetWheelVertically()        const   {return Eve.wheel.y;}
        int GetWheelHorizontally()      const   {return Eve.wheel.x;}
    };

    struct EventBox
    {
        enum EventStyle {Window=0,Mouse,Key,Wheel,Text,Max};
        void StoreEvent(const std::shared_ptr<Event>&);
        const std::vector<std::shared_ptr<Event>>& GetEventBox(EventStyle L) const {return Box[L];}
        void Clear();
        const std::vector<std::shared_ptr<Event>>& GetWindowBox()   const   {return GetEventBox(Window);}
        const std::vector<std::shared_ptr<Event>>& GetMouseBox()    const   {return GetEventBox(Mouse);}
        const std::vector<std::shared_ptr<Event>>& GetKeyBox()      const   {return GetEventBox(Key);}
        const std::vector<std::shared_ptr<Event>>& GetWheelBox()    const   {return GetEventBox(Wheel);}
        const std::vector<std::shared_ptr<Event>>& GetTextBox()     const   {return GetEventBox(Text);}
    private:
        std::vector<std::shared_ptr<Event>> Box[Max];
    };
}
#endif
