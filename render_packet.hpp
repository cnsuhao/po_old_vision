#ifndef RENDER_PACKET_HPP_INCLUDE
#define RENDER_PACKET_HPP_INCLUDE
#include "render_packet.h"
#include <typeinfo>
namespace PO
{
    namespace PO_Inner_Tool
    {
        MEMBER_FUNCTION_DETECTOR(REN_PAC,PosRender,void,(R,POC),Render& R,POContext& POC);
        MEMBER_FUNCTION_DETECTOR(REN_PAC,PreRender,void,(R,POC),Render& R,POContext& POC);
        MEMBER_FUNCTION_DETECTOR(REN_PAC,ConDelete,void,());
        MEMBER_FUNCTION_DETECTOR(REN_PAC,ConCreate,void,());
        MEMBER_FUNCTION_DETECTOR(REN_PAC,NorPluCreate,void,(Re,POC,TInf),Render& Re,POContext& POC,const std::type_info& TInf);
        MEMBER_FUNCTION_DETECTOR(REN_PAC,NorPluDelete,void,(Re,POC,TInf),Render& Re,POContext& POC,const std::type_info& TInf);
        TYPEDEF_DETECTOR( PrePlugin );
        //TYPEDEF_DETECTOR( ModAdapter );
        TYPEDEF_DETECTOR( PosPlugin );

        template<bool B,typename T> template<typename P> void PrePluginCreater<B,T>::Create(P& L,typename P::RenAdapter& Ad){L.PrePluginList.push_back(Render_Modual_Creater<typename T::PrePlugin,P>::Create(Ad));}
        template<typename T>  struct PrePluginCreater<false,T>{template<typename P> static void Create(P& L,typename P::RenAdapter& Ad){};};
        template<bool B,typename T> template<typename P> void PosPluginCreater<B,T>::Create(P& L,typename P::RenAdapter& Ad){L.PosPluginList.push_back(Render_Modual_Creater<typename T::PosPlugin,P>::Create(Ad));}
        template<typename T>  struct PosPluginCreater<false,T>{template<typename P> static void Create(P& L,typename P::RenAdapter& Ad){};};

        template<typename T> template<typename T2,typename T3> void CreatePrePlugin<T>::Run(T& t,T2& t2,T3& t3){  PrePluginCreater<TYPEDEF_DETECT_RESOULT(T,PrePlugin),T> :: Create(t2,t3);};
        template<typename T> template<typename T2,typename T3> void CreatePosPlugin<T>::Run(T& t,T2& t2,T3& t3){  PosPluginCreater<TYPEDEF_DETECT_RESOULT(T,PosPlugin),T> :: Create(t2,t3);};

        template<typename T>  void PreRenRun<T>::Run(T& t,Render& R,POContext& P){MEMBER_FUNCTION_PROCESS(REN_PAC,PreRender,T,t,R,P);};
        template<typename T>  void PosRenRun<T>::Run(T& t,Render& R,POContext& P){MEMBER_FUNCTION_PROCESS(REN_PAC,PosRender,T,t,R,P);};

        template<typename T>  void ContextDeleteRun<T>::Run(T& t){MEMBER_FUNCTION_PROCESS(REN_PAC,ConDelete,T,t);};
        template<typename T>  void ContextRebuildRun<T>::Run(T& t){MEMBER_FUNCTION_PROCESS(REN_PAC,ConCreate,T,t);};
        template<typename T>  void PluCreateRun<T>::Run(T& t,Render& R,POContext& P,const std::type_info& TI){MEMBER_FUNCTION_PROCESS(REN_PAC,NorPluCreate,T,t,R,P,TI);};
        template<typename T>  void PluCloseRun<T>::Run(T& t,Render& R,POContext& P,const std::type_info& TI){MEMBER_FUNCTION_PROCESS(REN_PAC,NorPluDelete,T,t,R,P,TI);};

        template<typename T,typename Ex>
        auto Render_Modual_Creater<T,Ex>::Create( typename Ex::RenAdapter& Ad) -> typename Ex::PluStoreType
        {
            typename Ex::PluStoreType Tem( new PO_Inner_Tool::Plugin_Packet<T,Ex>(Ad) );
            return Tem;
        }

        template<typename ...Expand> template<typename T>
            Render_Modual<Expand...>::Render_Modual(Render& R,POContext& POC,T& t):UnbalanceTreeInheritance<Expand...>(t)
            {
                Adapter<Expand...> A1(*this);
                auto A2=A1.Connect(R);
                auto A3=A2.Connect(POC);
                RenAdapter A4(A3);
                UnbalanceTreeRun<CreatePrePlugin>::FindPosRun(GetMaxLevel(),*this,A4);
                UnbalanceTreeRun<CreatePosPlugin>::FindPreRun(GetMaxLevel(),*this,A4);
            }

        template<typename Expand>
            void Render_Packet<Expand>::RenderLoop(POContext& P)
            {
                SetCurrent();
                typename Expand::ModAdapter AdaT(*this);
                auto Ada=AdaT.Connect(P);
                PreRender(P);

                bool PluginExist=false;

                //DeliverEvent
                ForEachPlugin_Module(Expand::PrePluginList,[this,&Ada](Plugin_Ptr<Expand>& L){ L.Respond(GetBox(),Ada);});
                ForEachPlugin(PluginList,P,[this,&Ada,&PluginExist](Plugin_Ptr<Expand>& L){ PluginExist=true;L.Respond(GetBox(),Ada);});
                ForEachPlugin_Module(Expand::PosPluginList,[this,&Ada](Plugin_Ptr<Expand>& L){ L.Respond(GetBox(),Ada);});
                CleanScreen();
                //Layer2D
                SetParallel();
                ForEachPlugin_Module(Expand::PrePluginList,[&Ada](Plugin_Ptr<Expand>& L){L.Layer2D(Ada);});
                ForEachPlugin(PluginList,P,[&PluginExist,&Ada](Plugin_Ptr<Expand>& L){ PluginExist=true;L.Layer2D(Ada);});
                ForEachPlugin_Module(Expand::PosPluginList,[&Ada](Plugin_Ptr<Expand>& L){ L.Layer2D(Ada);});
                //Layer3D
                SetPerspective();
                ForEachPlugin_Module(Expand::PrePluginList,[&Ada](Plugin_Ptr<Expand>& L){ L.Layer3D(Ada);});
                ForEachPlugin(PluginList,P,[&PluginExist,&Ada](Plugin_Ptr<Expand>& L){ PluginExist=true;L.Layer3D(Ada);});
                ForEachPlugin_Module(Expand::PosPluginList,[&Ada](Plugin_Ptr<Expand>& L){ L.Layer3D(Ada);});
                FinishLoop();
                PosRender(P);
                SwapScreen();
                if(!PluginExist)
                    ExitRender();
            }

            template<typename Expand> template<typename T>
            void Render_Packet<Expand>::ForEachPlugin(typename Expand::PluStoreListType& PP,POContext& POC,const T& t)
            {
                for(auto L=PP.begin();L!=PP.end();)
                {
                    if((*L)->IsPluginExit())
                    {
                        PluClose(POC,typeid(*(*L)));
                        PP.erase(L++);
                        continue;
                    }else
                        t(*(*L));
                    ++L;
                }
            }

                template<typename Expand> template<typename T>
            void Render_Packet<Expand>::ForEachPlugin_Module(typename Expand::PluStoreListType& PP,const T& t)
            {
                for(auto L=PP.begin();L!=PP.end();)
                {
                    if((*L)->IsPluginExit())
                    {
                        PP.erase(L++);
                        continue;
                    }else
                        t(*(*L));
                    ++L;
                }
            }

        template<typename T> template<typename Expand,typename ...AP>
            std::shared_ptr<Plugin_Packet<T,Expand>> Render_Creater<T>::Create(Render_Packet<Expand>& RP,POContext& POC,AP&... p)
            {
                RP.SetCurrent();
                std::shared_ptr<Plugin_Packet<T,Expand>> Tem( new Plugin_Packet<T,Expand>(p...) );
                typename Expand::PluStoreType Pc(Tem);
                RP.PluginList.push_back(Pc);
                RP.PluCreate(POC,typeid(*Tem));
                return Tem;
            };
    }

}
#endif // RENDER_PACKET_HPP_INCLUDE
