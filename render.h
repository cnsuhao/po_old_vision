#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED
#include <memory>
#include <functional>
#include "graphic/graphic.h"
#include "templatemetaprogramming/inheritance.h"
#include "templatemetaprogramming/adapter.h"
#include "elementfigure.h"
namespace PO
{
    class POContext;
    class Render:public GraphicContext
    {
        friend class POContext;

        bool ExitRenderSignal=false;
        bool ExitDirect=true;
        bool PreExitSignal=false;
        EventBox EB;
        virtual void RenderLoop(POContext&)=0;

        bool CheckExitSignal() const {return ExitRenderSignal;}
        Render(const Render&)=delete;
        Render& operator= (const Render&) =delete;
    protected:
        void RecoverRender(){ExitRenderSignal=false;PreExitSignal=false;}
        Render(const GraphicInfo& GI):GraphicContext(GI){}
        void GetEvent( const std::shared_ptr<Event>& L )
        {
            if(L->IsWindowClose())
            {
                PreExitSignal=true;
                if(ExitDirect)
                    ExitRenderSignal=true;
            }
            EB.StoreEvent(L);
        }
        void FinishLoop(){EB.Clear();}
        const EventBox& GetBox()const {return EB;}
        //virtual void PreWinChange()=0;
        //virtual void PosWinChange(const GraphicInfo&,const GraphicInfo&)=0;
    public:
        virtual void ResetWindowSize(int X,int Y)=0;
        GraphicInfo GetGraphicInfo() const {GraphicInfo GI(*this);return GI;}
        typedef Adapter<POContext,Render> ModualAdapter;
        void ExitRender(){PreExitSignal=true;if(ExitDirect)ExitRenderSignal=true; }
        virtual ~Render(){}
        void RespondExitSingal(){ExitDirect=false;}
        void IgnoreExitSignal(){ExitDirect=true;}
        void GetMouseLocation(float& X,float &Y,const std::shared_ptr<Event>& P)const
        {
            int Xi,Yi;
            P->GetMouse(Xi,Yi);
            PointCalculate(Xi,Yi,X,Y);
        }
        void GetMouse( Vertex<2>& P,const std::shared_ptr<Event>& EP )const {GetMouseLocation(P(0),P(1),EP); }
    };

    /*class Draw_Texture
    {
        bool Ready;
        void* operator new(size_t i)=delete;
    public:
        operator bool () const {return Ready;}
        Draw_Texture(Render& R,const GraphicTexture& GT):Ready(R.Tex_2D_En)
        {
            if(!Ready)
            {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D,GT.GetPoint());
                Ready=true;
            }
        }
        ~Draw_Texture()
        {
            if(Ready)
            {
                glDisable(GL_TEXTURE_2D);
                Ready=false;
            }
        }
    };*/

    inline GraphicInfo GetGraphicInfo( Render& L) {return L.GetGraphicInfo();}
}


#endif // RENDERBASE_HPP_INCLUDED
