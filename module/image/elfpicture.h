#ifndef ELFPICTURE_H_INCLUDED
#define ELFPICTURE_H_INCLUDED
#include "popicture.h"
namespace PO
{
    struct ElfDrawRule
    {
        virtual ~ElfDrawRule();
        virtual void InputMessage(int){};
        virtual void ChangeState(long,void*)=0;
    };

    class ElfPicture:public Picture
    {
    public:
        int WC=0,HC=0;

        void LoadElf(ImageManager& IM,const std::string& L,int W,int H,float Size);
        void DrawElf(Render& IM){DrawPicture(IM);}
    };

    namespace ElfTool
    {
        void ElfSetRect(ElfPicture&,float SX,float SY,float Size);
        void SetState(ElfPicture& ,int W,int Y);
    }

    /*namespace Pattern
    {
        namespace Elv{
            void SetEqualRectangle(ElfPicture& EP,float Size,const Vertex<2>& Shift=Ver(0.0,0.0),const Color<4>& C=Col(1.0,1.0,1.0,1.0),int W=0,int H=0);
            void SetState(ElfPicture& EP,int W,int H);
            inline void Reset( ElfPicture& EP,const Vertex<2>& N){Pattern::Pic::Reset(EP.P,N);}
            inline void Reset( ElfPicture& EP,const Color<4>& C){Pattern::Pic::Reset(EP.P,C);}
        }
    }*/
}


#endif // ELFPICTURE_H_INCLUDED
