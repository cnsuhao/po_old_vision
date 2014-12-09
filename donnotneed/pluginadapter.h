#ifndef PLUGINADAPTER_H_INCLUDED
#define PLUGINADAPTER_H_INCLUDED
namespace PO
{
    template<typename T,typename ...AO>
    class PluginAdapterPlu:public PluginAdapterPlu<AO...>
    {
        T& This;
    public:
        template<typename Type> Type& Get(){return static_cast<Type&>(*this);}
        operator T& () {return This;}
        template<typename Un>
            PluginAdapterPlu(Un& U):PluginAdapterPlu<AO...>(U),This(U){}
    };

    template<typename T>
    class PluginAdapterPlu<T>
    {
        T& This;
    public:
        template<typename Type> Type& Get(){return static_cast<Type&>(*this);}
        operator T& () {return This;}
        template<typename Un>
            PluginAdapterPlu(Un& U):This(U){}
    };

    template<typename Type,typename ...AP>
    class PluginAdapter:public PluginAdapterPlu<Type,AP...>
    {
        Render* Data;
    public:
        template<typename ...Modual>
            PluginAdapter( Render_Packet<Modual...>& u):Data(&static_cast<Render&>(u)),PluginAdapterPlu<Type,AP...>(u){}
        template<typename ...Modual>
            PluginAdapter( const std::shared_ptr<Render_Packet<Modual...>>& u):Data(&static_cast<Render&>(*u)),PluginAdapterPlu<Type,AP...>(*u){}
    };
}

#endif // PLUGINADAPTER_H_INCLUDED
