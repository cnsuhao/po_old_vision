#ifndef POINTASSEMBLE_HPP_INCLUDED
#define POINTASSEMBLE_HPP_INCLUDED
#include "glentity.hpp"
template<typename T>
class PointAssembleDataBase:virtual public HandleSeed
{
public:
    virtual unsigned int GetFormalSize() const =0;
    virtual const T* RealData()   const   =0;
};

template<typename T,unsigned int S>
class PointAssembleData:public PointAssembleDataBase
{
    std::vector<T> Data;
public:
    PointAssembleData(){}
    PointAssembleData(const std::initiation_list<T>& L):Data(L){}
    void Add(const Vector<T,S>& Ve)         {for(int i=0;i<Type;++i)Data.push_back(Ve(i));}
    void Add(const T* DA,unsigned int I)    {unsigned int K=Data.size();Data.resize(K+I);for(int i=K;i<Data.size();++i)Data[i]=DA[i-K];}
    const T* RealData()  const              {return &Data[0];}
    unsigned int GetFormalSize() const      {return S}
    unsigned int Size()const                {return Data.size();}
    void Clear()                            {Data.clear();Type=0;}
    T* GetData()                            {return &Data[0];}
};

template<typename T,unsigned int S>
class PointAssemble:public COWTech < AssembleData<T,S> >
{
public:
    PointAssemble(){}
    PointAssemble(const std::initiation_list<T>& L):COWTech < AssembleData<T,S> >(L){}
    void Add(const Vector<T,S>& Ve)     {this->GetData()->Add(Ve);}
    void Add(const T* DA,unsigned int I)    {this->GetData()->Add(DA,I);}
    void Clear()                            {this->GetData()->Clear();}
    Assemble& operator= (const Assemble& Tem){COWTech<AssembleData<T> >::operator=(Tem);return *this;}
};

class PointPattern
{
    Handle<PointAssembleDataBase> Loc;
    Handle<PointAssembleDataBase> Col;
    Handle<PointAssembleDataBase> Nor;
    Handle<PointAssembleData<unsigned int,1> > Draw;
public:
    const float* LocData()const{return Loc->RealData();}
    const float* ColData()const{return Loc->RealData();}
    const float* NorData()const{return Loc->RealData();}
    const unsigned int* DrawData()const{return Draw->RealData();}
    const unsigned int DrawCount()const{return Draw->Size();}
    void ConnectLoc(const Handle<PointAssembleDataBase>& L){Loc=L;}
    void ConnectCol(const Handle<PointAssembleDataBase>& L){Col=L;}
    void ConnectNor(const Handle<PointAssembleDataBase>& L){Nor=L;}
    void ConnectDraw(const Handle<PointAssemble<unsigned int , 1> >& L){Nor=L;}
};

#endif // POINTASSEMBLE_HPP_INCLUDED
