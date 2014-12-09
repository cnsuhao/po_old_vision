#ifndef PLUGIN_H
#define PLUGIN_H
#include "elementfigure.h"
#include "templatemetaprogramming/typeselect.h"
#include "templatemetaprogramming/adapter.h"
#include "render.h"

/*
template<typename ...T> class Plugin is the base type of all the plugin used in the this framework.
So, if wanting to create a plugin, a user-define class which derivatives by Plugin<XX...> needed to be defined.
The XX... means the module or part of then render that this plugin interested.
The follow type is available:
    Render : To control the Render that this Plugin belows to, it can get the location of mouse, the screen coordinate
            and other information about the window.
    POContext : To control the SDL environment, creating new Render, and creating new Plugin(new Plugin can be Created only
            in the situation with both Render and POContext exist).
    Other Module : If Creating Render with A module and B module, That means both A and B is available.

    Warning : the module you define in Plugin<XXX...>, must define in Render first.

By deriving class Plugin<XX...>, a defined type " PluAdapter " can be use.
By using PluAdapter, the reference of module can be casted.
For example:
    If deriving class Plugin<A,B,C>, A& U(PluAdapter) is fine.

    Warning : The above reference can be cast automatically once. Is means class A:public B; Plugin<A>; B& U(PluAdapter)
        is not correct.

After deriving Plugin, those following functions can be detect:
    void Respond( const EventBox& , PluAdapter ):Event Respond function. The Event class is define in "po/graphic/event.h"
            Painting in this function would not cause compiling error, but what it draw will not be display.
            Because this function will be call before calling screen clean function.
    void Layer2D(PluAdapter ):Draw 2D.
    void Layer3D(pluAdapter ):Draw 3D, the difference with the upper function is calling the different matrix.

Creating Plugin inside Plugin is available after deriving template<typename ...T> PluginCreater<T...>.
    (This derivation is not necessary if you do not need to create new plugin inside the plugin)
By deriving PluginCreater<A>, calling function CreatePlugin<A>( PluAdapter() )  can create a new Plugin and return
    shared_ptr<A>. This Function will call the construction of A(PluAdapter()) by default. So if wanting call other
    construction, PluginCreater should be derived likes PluginCreater<Creater<T,T1,T2,T3>>.
In class Creater<T,T1,T2,T3>, the first type means the plugin name, and the following type is the type of construction.
    function CreatePlugin<T>( T1(),T2(),T3() ) should be call like this.
If wanting to call construct T(PluAdapter,int), derived class should be like PluginCreater< Creater<T,PluAdapter,int> >.

Warning : CreatePlugin<T>(...) This function Could Not Call In Construction Function.

By the way, Creating new Plugin DO NOT NEED TO KNOW the real define of plugin type, that means if wanting to create
    plugin TII, just write class TII; before the define.


*/

namespace PO
{
    class POContext;
    template<typename ...AP>    struct Plugin;

    namespace PO_Inner_Tool
    {
        class PluginBase
        {
            bool Exit;
            PluginBase():Exit(false){}
            template<typename ...AP> friend class PO::Plugin;
        public:
            bool IsExit()const {return Exit;}
            void ExitPlugin(){Exit=true;}
            virtual ~PluginBase(){}
            void GetMouseLocation(Render& L,Vertex<2>& Mouse,const std::shared_ptr<Event>& E){L.GetMouseLocation(Mouse(0),Mouse(1),E);}
            void CloseRender(Render& L){L.ExitRender();}
            void GetScreen2D(Render& L,float& X,float& Y){X=L.GetScreen2DX();Y=L.GetScreen2DY();}
            void SetScreen2D(Render& L,float X,float Y){ L.Set2DView(X,Y);    }
            long GetSpendTime(Render& L){return L.GetSpendTime();}
            long GetIntervalTime(Render& L){return L.GetIntervalTime();}
            void ResetWindowSize(Render& L,int X,int Y){L.ResetWindowSize(X,Y);}
            void GetWindowSize(Render& L,int& X,int& Y){X=L.GetWindowW();Y=L.GetWindowH();}
            void ExitRender(Render& L){L.ExitRender();}
        };
    }

    template<typename ...AP>
    struct Plugin:public PO_Inner_Tool::PluginBase
    {
        typedef Adapter<POContext,Render,AP...> PluAdapter;
        Plugin(){}
        void Respond(const EventBox& ,PluAdapter L){}
        void Layer2D(PluAdapter L){}
        void Layer3D(PluAdapter L){}
    };

    template<typename ...AP> class PluCreater;
    class ThoseAdapter{};

    namespace PO_Inner_Tool
    {
        template<typename This>
        struct VirtualCreater
        {
            virtual std::shared_ptr<This> VirtualPluginCreater(Adapter<POContext,Render>&,This* )=0;
        };

        template<typename This,typename ...P>
        struct VirtualCreater<  PO::PluCreater<This,P...>    >
        {
            virtual std::shared_ptr< This >
                VirtualPluginCreater(  Adapter<POContext,Render>&,This*,P...)=0;
        };

        template<typename This,typename ...AP>
        struct PluginCreaterPlu:public VirtualCreater<This>,public PluginCreaterPlu<AP...>{};
        template<typename This>
        struct PluginCreaterPlu<This>:public VirtualCreater<This>{};
    }

    template<typename ...AL>
    struct PluginCreater:public PO_Inner_Tool::PluginCreaterPlu<AL...>
    {
        template<typename Type,typename ...P> std::shared_ptr<Type> CreatePlugin(Adapter<POContext,Render> R,P... p)
        {
            return this->VirtualPluginCreater(R,(Type*)(nullptr),p...);
        }
        typedef PluginCreater creater_type;
    };
}
#endif // PLUGIN_HPP
