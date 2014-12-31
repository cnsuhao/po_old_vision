#ifndef PORENDER_PACKET_H
#define PORENDER_PACKET_H
#include "render.h"
#include "plugin_packet.h"
#include "templatemetaprogramming/typeselect.h"
#include "templatemetaprogramming/detect.h"
namespace PO
{
    namespace PO_Inner_Tool
    {
        template<bool ,typename T> struct PrePluginCreater{template<typename P> static void Create(P& L,typename P::RenAdapter& Ad);};
        template<bool ,typename T> struct PosPluginCreater{template<typename P> static void Create(P& L,typename P::RenAdapter& Ad);};

        template<typename T> struct CreatePrePlugin{ template<typename T2,typename T3> static void Run(T& t,T2& t2,T3& t3);};
        template<typename T> struct CreatePosPlugin{ template<typename T2,typename T3> static void Run(T& t,T2& t2,T3& t3);};

        template<typename T>  struct PreRenRun{ static void Run(T& t,Render&,POContext&);};
        template<typename T>  struct PosRenRun{ static void Run(T& t,Render&,POContext&);};
        template<typename T>  struct ContextDeleteRun{ static void Run(T& t);};
        template<typename T>  struct ContextRebuildRun{ static void Run(T& t);};
        template<typename T>  struct PluCreateRun{ static void Run(T& t,Render& R,POContext& POC,const std::type_info& TI);};
        template<typename T>  struct PluCloseRun{ static void Run(T& t,Render& R,POContext& P,const std::type_info& TI);};

        template<typename ...Expand> struct Render_Modual:public UnbalanceTreeInheritance<Expand...>
        {
            typedef Adapter<Expand...,Render> ModAdapter;
            typedef Adapter<Expand...,Render,POContext> RenAdapter;
            typedef std::shared_ptr<Plugin_Ptr<Render_Modual<Expand...> > > PluStoreType;
            typedef std::list<PluStoreType > PluStoreListType;

            PluStoreListType PrePluginList;
            PluStoreListType PosPluginList;

            UnbalanceTreeInheritance<Expand...>& GetMaxLevel() {return *this;}
            template<typename T>    Render_Modual(Render& R,POContext& POC,T&& t);
        };

        template<typename Expand>
            class Render_Packet:public Render,public Expand
            {
                template<typename T> friend class Render_Creater;
                void PreRender(POContext& POC)    {UnbalanceTreeRun<PreRenRun>::FindPosRun( Expand::GetMaxLevel(),*this,POC);}
                void PosRender(POContext& POC)    {UnbalanceTreeRun<PosRenRun>::FindPreRun( Expand::GetMaxLevel(),*this,POC);}
                void PluCreate(POContext& P,const std::type_info& TI)   {UnbalanceTreeRun<PluCreateRun>::FindPosRun(Expand::GetMaxLevel(),*this,P,TI);};
                void PluClose(POContext& P,const std::type_info& TI)    {UnbalanceTreeRun<PluCloseRun>::FindPreRun(Expand::GetMaxLevel(),*this,P,TI);};
                void ContextDelete()                                {UnbalanceTreeRun<ContextDeleteRun>::FindPosRun(Expand::GetMaxLevel());};
                void ContextRebuild()                               {UnbalanceTreeRun<ContextRebuildRun>::FindPreRun(Expand::GetMaxLevel());};
                typename Expand::PluStoreListType PluginList;
                template<typename T> void ForEachPlugin(typename Expand::PluStoreListType& PP,POContext& POC,const T& t);
                template<typename T> void ForEachPlugin_Module(typename Expand::PluStoreListType& PP,const T& t);
            public:
                const GraphicInfo& GetGraphicInfo()const            {return *this;}
                void RenderLoop(POContext& );
                void ResetWindowSize(int X,int Y){ContextDelete();GraphicContext::ResetWindowSize(X,Y);ContextRebuild();}
                template<typename UnknowType> Render_Packet( GraphicInfo& GI,POContext& POC,UnknowType&& UT  ):Render(GI),Expand(*this,POC,std::forward<UnknowType>(UT)){}
            };

        template<typename T,typename Ex>struct Render_Modual_Creater
        {
            auto static Create(typename Ex::RenAdapter&) -> typename Ex::PluStoreType;
        };

        template<typename T> struct Render_Creater
        {
            template<typename Expand,typename ...AP>    static auto Create( Render_Packet<Expand>& RP,POContext& POC,AP&... p ) -> std::shared_ptr<Plugin_Packet<T,Expand>>;
        };
    }



    template<typename T,typename Ren,typename ...Other,typename ...AP>
        auto CreatePlugin(  Adapter<Ren,Other...>& L,AP&... ap) ->
            decltype( PO_Inner_Tool::Render_Creater<T>::Create( static_cast<Ren&>(L),L,ap...))
            {return PO_Inner_Tool::Render_Creater<T>::Create( static_cast<Ren&>(L),L,ap...);}

}
#endif // PORENDER_HPP
