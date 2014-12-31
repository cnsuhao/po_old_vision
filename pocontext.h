#ifndef POCONTEXT_H_INCLUDED
#define POCONTEXT_H_INCLUDED
#include <ostream>
#include "module/pomodule.h"
#include "render_packet.h"
#include "templatemetaprogramming/adapter.h"
namespace PO
{
    struct POContextIniter
    {
        POContextIniter(){Graphic::Init();}
        ~POContextIniter(){Graphic::Quit();}
    };

    class POContext:public POContextIniter
    {
        template<typename T>        friend int Loop(const T&);
        template<typename Tem>      friend int Loop(std::ostream& OS,const Tem&);
        friend int Loop(std::ostream& OS,void (*)(POContext&));
        std::map<int,std::shared_ptr<Render> > RenderMap;
        long SpendTime;
        POContext() {SpendTime=Graphic::GetTick();}
    public:

        template<typename ...Type,typename ...Pa>
            auto CreateRender( const std::shared_ptr<GraphicInfo>& GI,TypePair< TypeInfo<Type...>,std::tuple<Pa...> >  && T  ) ->  Adapter<PO_Inner_Tool::Render_Packet< PO_Inner_Tool::Render_Modual<Type...> >,Render,Type...,POContext>;
        auto CreateRender( const std::shared_ptr<GraphicInfo>& GI ) -> Adapter<PO_Inner_Tool::Render_Packet< PO_Inner_Tool::Render_Modual<EmptyModual> >,Render,EmptyModual,POContext>
        {
            return CreateRender(GI,CreateModule<EmptyModual>());
        }
        void DeliverEvent( const std::shared_ptr<Event>& Tem );
        int ContextLoop();
    };

    template<typename ...Type,typename ...Pa>
    auto POContext::CreateRender( const std::shared_ptr<GraphicInfo>& GI, TypePair< TypeInfo<Type...>,std::tuple<Pa...>   >&& T    ) -> Adapter<PO_Inner_Tool::Render_Packet< PO_Inner_Tool::Render_Modual<Type...> >,Render,Type...,POContext>
    {
        std::shared_ptr<PO_Inner_Tool::Render_Packet<PO_Inner_Tool::Render_Modual<Type...> >  > Tem(new PO_Inner_Tool::Render_Packet< PO_Inner_Tool::Render_Modual<Type...> >( *GI,*this,T.Data ));
        std::shared_ptr<Render> List(Tem);
        int i=List->GetWindowIndex();
        RenderMap[i]=List;
        Adapter<PO_Inner_Tool::Render_Packet< PO_Inner_Tool::Render_Modual<Type...> >,Render,Type...> TR(*Tem);
        return TR.Connect(*this);
    }
}


#endif // POCONTEXT_HPP_INCLUDED
