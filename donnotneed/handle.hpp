#ifndef HANDLE_HPP
#define HANDLE_HPP
#include <stdio.h>
class HandleSeed
{
public:
    bool Exist;
    unsigned int Using;
    virtual ~HandleSeed() {}
    unsigned int GetUsing() const {return Using;}
    bool CheckExist() const {return Exist;}
    void Destory(){Exist=false;}
    HandleSeed():Exist(true),Using(0) {}
};

template <class T>
class Handle
{
public:
    bool Used;
    T* Data;
    Handle<T>& operator =(T* L);
    Handle<T>& operator =(const Handle&);
    void Unused();
    bool Exist() const
    {
        if(Used)
            return Data->HandleSeed::CheckExist();
        return false;
    }
    operator T*(void){return Data;}
    operator const T* (void)const {return Data;}
    T& operator* () {return *Data;}
    const T& operator* () const {return *Data;}
    T* operator->() {return Data;}
    const T* operator->() const {return Data;}
    Handle():Used(false),Data(0) {}
    Handle(const Handle& H);
    Handle(T* L):Used(false),Data(0)
    {
        if(L)*this=L;
    }
    virtual ~Handle()
    {
        Unused();
    }
    template <class G>  Handle<T>& Back_Cast(const Handle<G>&);
};
template <class T> template <class G>
Handle<T>& Handle<T>::Back_Cast(const Handle<G>& H)
{
    Unused();
    if(H.Exist())
    {
        T* Tem=dynamic_cast<T*>(H.Data);
        if(Tem!=nullptr)
            *this=Tem;
    }
    return *this;
}

template<class T>
Handle<T>& Handle<T>::operator =(T* L)
{
    Unused();
    if(L)
    {
        Used=true;
        L->HandleSeed::Using+=1;
        Data=L;
    }
    return *this;
}

template<class T>
Handle<T>& Handle<T>::operator =(const Handle<T>& H)
{
    if(H.Exist())
    {
        T* Tem=H.Data;
        Tem->HandleSeed::Using+=1;
        *this=Tem;
        Tem->HandleSeed::Using-=1;
    }
    else
        Unused();
    return *this;
}

template<class T>
Handle<T>::Handle(const Handle &H)
{
    Used=H.Used;
    Data=H.Data;
    if(H.Used)
        Data->HandleSeed::Using+=1;
}
template <class T>
void Handle<T>::Unused()
{
    if(Used)
    {
        Data->HandleSeed::Using-=1;
        if(Data->HandleSeed::Using<=0)
            delete Data;
        Used=false;
    }
}

template<typename T>
class COWTech
{
    Handle<T> Data;
public:
    void Copy()
    {
        if(Data->GetUsing()>1)
        {
            Handle<T> Tem=new T(*Data);
            Data=Tem;
        }
    }
    void New()
    {
        if(Data->GetUsing()>1)
        {
            Handle<T> Tem=new T();
            Data=Tem;
        }
    }
    COWTech<T>& operator= (  const COWTech<T>& L ) {Data=L.Data;return *this;}
    COWTech(const COWTech& CW):Data(CW.Data){}
    template<typename ...TO>
        COWTech(const TO&... TP){Data=new T(TP...);}
    const Handle<T>& GetData() const {return Data;}
    Handle<T>& GetData()
    {
        Copy();
        return Data;
    }

    //Handle<T>&  GetData(){return Data;}
};


#endif // HANDLE_H
