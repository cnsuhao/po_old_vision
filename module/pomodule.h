#ifndef POMODULE_H_INCLUDED
#define POMODULE_H_INCLUDED
#include <list>
#include <memory>
#include <tuple>
#include "../templatemetaprogramming/inheritance.h"

/*
Those following function could be detected of module:
    void NorPluCreate( PO::Render&,PO::POContext&,const std::type_info&);
        //This function will be call before a new plugin put inside the list.

    void NorPluDelete( PO::Render&,PO::POContext&,const std::type_info&);
        //This function will be call before a new plugin put inside the list.

    void PreRender(PO::Render&,PO::POContext&);
        //This function will be call in the start of each loop.( before the respond function of plugin );

    void PosRender(PO::Render&,PO::POContext&);
        //This function will be call in the end of each loop.( after the Layer3D function of Plugin );


Those follow type could be detected as a kind of special plugin:
    PrePlugin;
    PosPlugin;

*/







namespace PO
{
    class EmptyModual{};
    template< typename Type ,typename ...AP>
        inline auto CreateGenerialModule( AP... ap) -> decltype(CreateTypePair<Type>(ap...)){Type::PreConstruct(ap...);return CreateTypePair<Type>( ap... );}
    template< typename Type ,typename ...AP>
        inline auto CreateModule( AP... ap) -> decltype(CreateTypePair<Type>(ap...)){return CreateTypePair<Type>( ap... );}
    template< typename ...Type>
        inline auto CreateModule() -> decltype(CreateTypePair<Type...>()){return CreateTypePair<Type...>();}

    /*template<typename ...T1,typename ...T2,typename ...I1,typename ...I2>
        inline auto IncludeModual(   const std::shared_ptr<TypePair<TypeInfo<T1...>,std::tuple<I1...>>>& TP1,
                        const std::shared_ptr<TypePair<TypeInfo<T2...>,std::tuple<I2...>>>& TP2)
    -> decltype(ConnectTypePair(TP1,TP2))
    {
        return ConnectTypePair(TP1,TP2);
    }*/

    template<typename ...ALLModule>
        struct ModuleType
        {
            typedef UnbalanceTreeInheritance<ALLModule...> type;
        };

    template<typename ...All,typename Type>
        UnbalanceTreeInheritance<All...>
            AbstractModuleType( const TypePair< TypeInfo<All...>,Type >& L  ){}
    template< typename Mod, template<typename Type> class Plu>
        UnbalanceTreeInheritance<Mod>
            AbstractModuleType( std::shared_ptr<Plu<UnbalanceTreeInheritance<Mod> >>& L ){}
}


#endif // POMODUAL_HPP_INCLUDED
