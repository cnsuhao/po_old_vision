#ifndef PLUGIN_PACKET_H_INCLUDED
#define PLUGIN_PACKET_H_INCLUDED
#include "plugin.h"
#include "render_packet.h"
namespace PO
{
    namespace PO_Inner_Tool
    {
        template<typename Expand> class Render_Packet;
        template<typename T> class Render_Creater;
        template<typename Expand>
        struct Plugin_Ptr
        {
            virtual bool IsPluginExit() const =0;
            virtual void Layer3D(typename Render_Packet<Expand>::RenAdapter& R)=0;
            virtual void Layer2D(typename Render_Packet<Expand>::RenAdapter& R)=0;
            virtual void Respond(const EventBox& EB,typename Render_Packet<Expand>::RenAdapter& R)=0;
        };

        template<typename Pre,typename Expand,typename This,typename ...AP>
            std::shared_ptr< This > Inside_PluginCreate( Adapter<POContext,Render>& DD,AP&&... ap);
        template<typename Pre,typename Expand,typename This,typename ...AP>
            std::shared_ptr< This > Inside_Default_PluginCreate( Adapter<POContext,Render>& DD);

        template<typename Pre,typename Expand,typename This,typename ...AP>
        struct RealCreater:public RealCreater<Pre,Expand,AP...>
        {
            std::shared_ptr<This> VirtualPluginCreater(Adapter<POContext,Render>& DD,This* )
            {
                return Inside_Default_PluginCreate<Pre,Expand,This>(DD);
            }
            template<typename ...AO>
                RealCreater(AO&&... a):RealCreater<Pre,Expand,AP...>(a...){}
        };

        template<typename Pre,typename Expand,typename This,typename ...P,typename ...AP>
        struct RealCreater<Pre,Expand,PO::PluCreater<This,P...>,AP...>:public RealCreater<Pre,Expand,AP...>
        {
            std::shared_ptr<This> VirtualPluginCreater(Adapter<POContext,Render>& L,This*,P... p)
            {
                return Inside_PluginCreate<Pre,Expand,This>(L,p...);
            }
            template<typename ...AO>
                RealCreater(AO&&... a):RealCreater<Pre,Expand,AP...>(a...){}
        };

        template<typename Pre,typename Expand,typename This>
        struct RealCreater<Pre,Expand,This>:public Pre
        {
            std::shared_ptr<This> VirtualPluginCreater(Adapter<POContext,Render>& DD,This* )
            {
                return Inside_Default_PluginCreate<Pre,Expand,This>(DD);
            }
            template<typename ...AP>
                RealCreater(AP&&... a):Pre(a...){}
        };

        template<typename Pre,typename Expand,typename This,typename ...P>
        struct RealCreater<Pre,Expand,PO::PluCreater<This,P...> >:public Pre
        {
            std::shared_ptr<This> VirtualPluginCreater(Adapter<POContext,Render>& L,This*,P... p)
            {
                return Inside_PluginCreate<Pre,Expand,This>(L,p...);
            }
            template<typename ...AP>
                RealCreater(AP&&... a):Pre(a...){}
        };

        template<typename Pre,typename Expand,typename ...AP>
            RealCreater<Pre,Expand,AP...> PluginCreater_Detector(  PluginCreater<AP...>*   );
        template<typename Pre,typename Expand>
            Pre PluginCreater_Detector(...);

        template<typename Type,typename Expand>
        struct Plugin_Packet
            :public std::enable_if<std::is_base_of< PluginBase ,Type >::value,decltype(PluginCreater_Detector<Type,Expand>( (Type*)(nullptr)  ))>::type,public Plugin_Ptr<Expand>
        {
            typedef decltype(PluginCreater_Detector<Type,Expand>( (Type*)(nullptr)  )) RealType;
            bool IsPluginExit() const {return Type::IsExit();}
            void Respond(const EventBox& EB, typename Render_Packet<Expand>::RenAdapter& L){Type::Respond(EB,L);}
            void Layer2D(typename Render_Packet<Expand>::RenAdapter& L){Type::Layer2D(L);}
            void Layer3D(typename Render_Packet<Expand>::RenAdapter& L){Type::Layer3D(L);}
            template<typename ...AD>
            Plugin_Packet( AD&&... ap ):RealType(ap...){}
        };

        template<typename Ada,typename This> This Inside_Translate( Ada& DD,This&& T){return T;}
        template<typename Ada> Ada& Inside_Translate( Ada& DD,PO::ThoseAdapter T){return DD;}

        template<typename Pre,typename Expand,typename This,typename ...AP>
            std::shared_ptr< This > Inside_PluginCreate( Adapter<POContext,Render>& DD,AP&&... ap)
            {
                Render& L=static_cast<Render&>(DD);
                Render_Packet<Expand>& Ex=dynamic_cast<Render_Packet<Expand>& >(L);
                typename Render_Packet<Expand>::ModAdapter TC(Ex);
                auto TC2=TC.Connect(static_cast<POContext&>(DD));
                return Render_Creater<This>::Create(Ex,DD,Inside_Translate(TC2,ap)...);//   Ex->PointCreate( DD, (This*)(nullptr) );
            }
        template<typename Pre,typename Expand,typename This,typename ...AP>
            std::shared_ptr< This > Inside_Default_PluginCreate( Adapter<POContext,Render>& DD)
            {
                Render& L=static_cast<Render&>(DD);
                Render_Packet<Expand>& Ex=dynamic_cast<Render_Packet<Expand>& >(L);
                typename Render_Packet<Expand>::ModAdapter TC(Ex);
                auto TC2=TC.Connect(static_cast<POContext&>(DD));
                return Render_Creater<This>::Create(Ex,DD,TC2);
            }
    }
    //template<typename Pre,typename This> This ParaTranslate(Pre::PluAdpater&,This );
    //template<typename Pre,typename This> Adapter<>


    /*template<typename Pre,typename Expand,typename This,typename ...AP> auto RealCreater<Pre,Expand,This,AP...>::VirtualPluginCreater(Adapter<POContext,Render>& DD,This* )->std::shared_ptr<This>
    {
        Render& L=static_cast<Render&>(DD);
        Render_Packet<Expand>* Ex=dynamic_cast<Render_Packet<Expand>*>(&L);
        typename Render_Packet<Expand>::ModAdapter TC(*Ex);
        auto TC2=TC.Connect(static_cast<POContext&>(DD));
        return Render_Creater<This>::Create(*Ex,TC2);//   Ex->PointCreate( DD, (This*)(nullptr) );
        //return Insid_PluginCreate(   )
    };

    template<typename Pre,typename Expand,typename This> auto RealCreater<Pre,Expand,This>::VirtualPluginCreater(Adapter<POContext,Render>& DD,This* )->std::shared_ptr<This>
    {
        Render& L=static_cast<Render&>(DD);
        Render_Packet<Expand>* Ex=dynamic_cast<Render_Packet<Expand>*>(&L);
        typename Render_Packet<Expand>::ModAdapter TC(*Ex);
        auto TC2=TC.Connect(static_cast<POContext&>(DD));
        return Render_Creater<This>::Create(*Ex,TC2);//   Ex->PointCreate( DD, (This*)(nullptr) );
    };*/
}

#endif // PLUGIN_PACKET_H_INCLUDED
