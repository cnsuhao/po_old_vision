#ifndef ELEMENTFIGURE_H_INCLUDED
#define ELEMENTFIGURE_H_INCLUDED
#include "templatemetaprogramming/vector.h"
#include "templatemetaprogramming/squarematrix.h"
template<unsigned int S>
class Normal : public Vector<float,S>
{
public:
    Normal(const Matrix<float,S,1>& P):Vector<float,S>(P)  {}
    Normal(float * K):Vector<float,S>(K)                   {}
    template<typename ...TO>
        Normal(const TO&... TP):Vector<float,S>(TP...)     {}
    Normal()                                               {}

    Normal operator+ (const Normal& L)    const       {Normal Tem( Vector<float,S>::operator+(L) );return Tem;}
    Normal operator- (const Normal& L)    const       {Normal Tem( Vector<float,S>::operator-(L) );return Tem;}

    Normal& operator+= (const Normal& L)              {Vector<float,S>::operator+=(L);return *this;}
    Normal& operator-= (const Normal& L)              {Vector<float,S>::operator-=(L);return *this;}

    Normal operator* (float K)           const     {Normal Tem(*this);for(int i=0;i<S;++i) Tem(i)*=K;return Tem;}
    float operator* (const Normal& K)    const     {return Vector<float,S>::operator*(K);}
    Normal& operator*= (float K)                   {for(int i=0;i<S;++i) this->operator()(i)*=K;return *this;}

    float VecLong2()                     const  {
                                                    float Tem=0;
                                                    for(int i=0;i<S;++i)
                                                        Tem+=this->operator()(i)*this->operator()(i);
                                                    return Tem;
                                                }

    Normal Unit()                        const     {
                                                    float VL=sqrt(VecLong2());
                                                    Normal Tem;
                                                    for(int i=0;i<S;++i)
                                                        Tem(i)/=VL;
                                                    return Tem;
                                                }
    void SelfUnit()                             {
                                                    float VL=sqrt(VecLong2());
                                                    for(int i=0;i<S;++i)
                                                        this->operator()(i)/=VL;
                                                }
};

template<unsigned int S>
    Normal<S> operator* (float K, const Normal<S>& L) {return L*K;}

template<unsigned int S>
struct Vertex : public Vector<float,S>
{
    Vertex(const Matrix<float,2,1>& P):Vector<float,S>(P)      {}
    Vertex(float * K):Vector<float,S>(K)                       {}
    template<typename ...TP>
        Vertex(const TP&... To):Vector<float,S>(To...)         {}
    Vertex()                                                     {}

    Vertex operator+ (const Normal<S>& L)  const       {Vertex Tem(Vector<float,S>::operator+(L));return Tem;}
    Vertex operator-  (const Normal<S>& L) const       {Vertex Tem(Vector<float,S>::operator-(L));return Tem;}
    Normal<S> operator- (const Vertex& L)const         {Normal<S> Tem(Vector<float,S>::operator-(L));;return Tem;}
    Vertex& operator+= (const Normal<S>& L)            {Vector<float,S>::operator+=(L);return *this;}
    Vertex& operator-= (const Normal<S>& L)            {Vector<float,S>::operator-=(L);return *this;}
    float Distance(const Vertex& K)    const        {return sqrt(this->operator-(K).VecLong2());}
};

template<unsigned int C>
struct Color:Vector<float,C>
{
    Color(const Matrix<float,C,1>& L):Vector<float ,C>(L){}
    Color(float* L):Vector<float,C>(L){}
    template<typename ...TP>
        Color(const TP& ... TO):Vector<float,C>(TO...){}
};

template<typename ...TP>
    Vertex<sizeof...(TP)> Ver(const TP&... TO) {return Vertex<sizeof...(TP)>(TO...);}
template<typename ...TP>
    Normal<sizeof...(TP)> Nor(const TP&... TO) {return Normal<sizeof...(TP)>(TO...);}
template<typename ...TP>
    Color<sizeof...(TP)> Col(const TP&... TO) {return Color<sizeof...(TP)>(TO...);}

/*template<unsigned int L,unsigned int C,unsigned int Li>
    struct GLPointEqualStruct
    {
            template<template< unsigned int Lk,unsigned int Ck >class P,typename TC,typename ...TP>
                static void Equal( P<L,C>& Lc,const TP&... TO){ LocEqual(Lc,TO...);  }
            template<template< unsigned int Lk,unsigned int Ck >class P,typename TC,typename ...TP>
                static void LocEqual( P<L,C>& Lc ,const TC& B, const TP&... TO){Lc.Loc(L-Li)=B; GLPointEqualStruct<L,C,Li-1>::LocEqual(Lc,TO...);}
            template<template< unsigned int Lk,unsigned int Ck >class P,typename TC,typename ...TP>
                static void ColEqual( P<L,C>& Lc ,const TC& B, const TP&... TO){Lc.Col(C-Li)=B;GLPointEqualStruct<L,C,Li-1>::ColEqual(Lc,TO...);}
            template<template< unsigned int Lk,unsigned int Ck >class P,typename TC,typename ...TP>
                static void NorEqual( P<L,C>& Lc ,const TC& B, const TP&... TO){Lc.Nor(3-Li)=B;GLPointEqualStruct<L,C,Li-1>::NorEqual(Lc,TO...);}
    };

template<unsigned int L,unsigned int C>
    struct GLPointEqualStruct<L,C,0>
    {
        template<template< unsigned int Lk,unsigned int Ck >class P,typename ...TP>
            static void LocEqual( P<L,C>& Li ,const TP&... TO){GLPointEqualStruct<L,C,C>::ColEqual(Li,TO...);}
        template<template< unsigned int Lk,unsigned int Ck >class P,typename ...TP>
            static void ColEqual( P<L,C>& Li , const TP&... TO){GLPointEqualStruct<L,C,3>::NorEqual(Li,TO...);}
        template<template< unsigned int Lk,unsigned int Ck >class P>
            static void NorEqual( P<L,C>& Li ){;}
    };

template<unsigned int L,unsigned int C>
class GLPoint
{
    Vertex<L> Loc;
    Color<C> Col;
    Normal<3>   Nor;
public:
    template<typename ...TP>  void SetLoc(const TP& ... TO) {Loc.Set(TO...);}
    template<typename ...TP>  void SetCol(const TP& ... TO) {Col.Set(TO...);}
    template<typename ...TP>  void SetNor(const TP& ... TO) {Nor.Set(TO...);}
    GLPoint() {}
    const Vertex<L>& GetLoc() const {return Loc;}
    const Color<C>& GetCol() const {return Col;}
    const Normal<3>& GetNor() const {return Nor;}
    GLPoint(const Vertex<L>& P,const Color<C>& Co,const Normal<3>& No):Loc(P),Col(Co),Nor(No){}
    template<typename ...TP>
        GLPoint(const TP&... TO) { GLPointEqualStruct<L,C,3>::Equal(*this,TO...);  }
};

template<typename T>
class AssembleData:public HandleSeed
{
    std::vector<T> Data;
    int Type=0;
public:
    void SetType(int Y){Type=Y;}
    template<unsigned int I>
        void Add(const Vector<T,I>& Ve)
        {
            for(int i=0;i<Type;++i)
                Data.push_back(Ve(i));
        }
    void Add(const T* DA,unsigned int I)
    {
        unsigned int K=Data.size();
        Data.resize(K+I);
        for(int i=K;i<Data.size();++i)
            Data[i]=DA[i-K];
    }
    unsigned int GetFormateSize()const{return Type;}
    unsigned int Size()const{return Data.size();}
    void Clear(){Data.clear();}
    T* GetData(){return &Data[0];}
};

template<typename T>
class Assemble:public COWTech < AssembleData<T> >
{
public:
    template<unsigned int I>
        void Add(const Vector<T,I>& Ve)
        {
            this->GetData()->Add(Ve);
        }
    void Add(const T* DA,unsigned int I)
    {
        this->GetData()->Add(DA,I);
    }
    void Clear()
    {
        this->GetData()->Clear();
    }
    Assemble& operator= (const Assemble& Tem)
    {
        COWTech<AssembleData<T> >::operator=(Tem);
        return *this;
    }
};


template<unsigned int L,unsigned int C>
struct GLPointAssembleData:public HandleSeed
{
    std::vector<float> Loc;
    std::vector<float> Col;
    std::vector<float> Nor;
    std::vector<unsigned int> DrawOrder;
    GLPointAssembleData(const GLPointAssembleData& GLPA):Loc(GLPA.Loc),Col(GLPA.Col),Nor(GLPA.Nor),DrawOrder(GLPA.DrawOrder){}
    GLPointAssembleData(){}

    void Clear()                        {Loc.clear();Col.clear();Nor.clear();DrawOrder.clear();}
    void ClearDraw()                    {DrawOrder.clear();}

    void Add(const GLPoint<L,C>& GLP)
    {
        for(int i=0;i<L;++i)        Loc.push_back(GLP.GetLoc()(i));
        for(int i=0;i<C;++i)        Col.push_back(GLP.GetCol()(i));
        for(int i=0;i<3;++i)        Nor.push_back(GLP.GetNor()(i));
    }

    void Add(const Vertex<L>& P,const Color<C>& Co,const Normal<3>& No)
    {
        for(int i=0;i<L;++i)        Loc.push_back(P(i));
        for(int i=0;i<C;++i)        Col.push_back(Co(i));
        for(int i=0;i<3;++i)        Nor.push_back(No(i));
    }
    void Add(const std::vector<float>& Lo,const std::vector<float>& Co,const std::vector<float>& No,const std::vector<unsigned int>& Do)
    {
        Loc.insert(Loc.end(),Lo.begin(),Lo.end());
        Col.insert(Col.end(),Co.begin(),Co.end());
        Nor.insert(Nor.end(),No.begin(),No.end());
        DrawOrder.insert(DrawOrder.end(),Do.begin(),Do.end());
    }
    void Add(const GLPointAssembleData& Lq)             {Add(Lq.Loc,Lq.Col,Lq.Nor,Lq.DrawOrder);}
    void DrawWith(const unsigned int* DR,unsigned int Cx)
    {
        DrawOrder.clear();
        for(int i=0;i<Cx;++i)
            DrawOrder.push_back(DR[i]);
    }

    GLPointAssembleData& operator= (const GLPointAssembleData& GA)
    {
        Loc=GA.Loc;
        Col=GA.Col;
        Nor=GA.Nor;
        DrawOrder=GA.DrawOrder;
        return *this;
    }
};


template<unsigned int L,unsigned int C>
class GLPointAssemble:COWTech<GLPointAssembleData<L,C>>
{
public:
    unsigned int DrawSize() const {return this->GetData()->DrawOrder.size();}
    const unsigned int* DrawData() const {return &this->GetData()->DrawOrder[0];}
    const float* DrawLoc() const {return &this->GetData()->Loc[0];}
    const float* DrawCol() const {return &this->GetData()->Col[0];}
    const float* DrawNor() const {return &this->GetData()->Nor[0];}

    void Creat2DSquare(
                            const Vertex<L>& P1,const Vertex<L>& P2, const Vertex<L>& P3,
                            const Color<C>& Co,const Normal<3>& Ve)
            {
                static const unsigned int Order[]={ 0,2,1,0,3,2 };
                this->Clear();
                Normal<L> T = P2 - P1 ;
                this->GetData()->Add( P1 , Co , Ve );
                this->GetData()->Add( P2 , Co , Ve );
                this->GetData()->Add( P3 , Co , Ve );
                this->GetData()->Add( P3 - T , Co , Ve );
                this->GetData()->DrawWith(Order,6);
            }

    void Clear()
    {
        this->New();
        this->GetData()->Clear();
    }
    void ClearDraw()
    {
        this->Copy();
        this->GetData()->ClearDraw();
    }
    void DrawWith(const unsigned int* DR,unsigned int Cx)
    {
        this->Copy();
        this->GetData()->DrawWith(DR,Cx);
    }


    void Add(const GLPoint<L,C>& GLP)                                       {this->Copy();this->GetData()->Add(GLP);}
    void Add(const Vertex<L>& P,const Color<C>& Co,const Normal<3>& No)         {this->Copy();this->GetData()->Add(P,Co,No);}
    void Add(const std::vector<float>& Lo,const std::vector<float>& Co,const std::vector<float>& No,const std::vector<unsigned int>& Do)
                                                                            {this->Copy();this->GetData()->Add(Lo,Co,No,Do);}
};*/

/*struct AxisRotate
{
    Normal<3> Axis;
    float Angel=0;
    AxisRotate():Axis(1.0,0.0,0.0),Angel(0){}
    AxisRotate(const Normal<3>& A,float An):Axis(A),Angel(An){Axis=Axis.Unit();}
    Normal<3> operator * (const Normal<3>& L) const
    {
        float   C=cos(Angel),
                S=sin(Angel),
                C1=1-C,
                XY=Axis(0)*Axis(1),
                XZ=Axis(0)*Axis(2),
                YZ=Axis(1)*Axis(2),
                XS=Axis(0)*S,
                YS=Axis(1)*S,
                ZS=Axis(2)*S;
        Normal<3> Tem (  (Axis(0)*Axis(0)*C1+C) * L(0) + (XY*C1-ZS) * L(1) + (XZ*C1+YS) *L(2) ,
                      (XY*C1+ZS) * L(0) + (Axis(1)*Axis(1)*C1+C) * L(1) + (YZ*C1+YS) *L(2) ,
                      (XZ*C1+ZS) * L(0) + (YZ*C1-ZS) * L(1) + (Axis(2)*Axis(2)*C1+C) *L(2) );
        return Tem;
    }

    Vertex<3> operator* (const Vertex<3>& L) const
    {
        float   C=cos(Angel),
                S=sin(Angel),
                C1=1-C,
                XY=Axis(0)*Axis(1),
                XZ=Axis(0)*Axis(2),
                YZ=Axis(1)*Axis(2),
                XS=Axis(0)*S,
                YS=Axis(1)*S,
                ZS=Axis(2)*S;
        Vertex<3> Tem (  (Axis(0)*Axis(0)*C1+C) * L(0) + (XY*C1-ZS) * L(1) + (XZ*C1+YS) *L(2) ,
                        (XY*C1+ZS) * L(0) + (Axis(1)*Axis(1)*C1+C) * L(1) + (YZ*C1+YS) *L(2) ,
                        (XZ*C1+ZS) * L(0) + (YZ*C1-ZS) * L(1) + (Axis(2)*Axis(2)*C1+C) *L(2) );
        return Tem;
    }

    Matrix<float,4,4> operator* (const AxisRotate& R)
    {

    }

    void TranslateToMatrix3(float* L)
    {
        float   C=cos(Angel),
                S=sin(Angel),
                C1=1-C,
                XY=Axis(0)*Axis(1),
                XZ=Axis(0)*Axis(2),
                YZ=Axis(1)*Axis(2),
                XS=Axis(0)*S,
                YS=Axis(1)*S,
                ZS=Axis(2)*S;
        L[0]=Axis(0)*Axis(0)*C1+C;
        L[1]=XY*C1+ZS;
        L[2]=XZ*C1-YS;
        L[3]=XY*C1-ZS;
        L[4]=Axis(1)*Axis(1)*C1+C;
        L[5]=YZ*C1+XS;
        L[6]=XZ*C1+YS;
        L[7]=YZ*C1-XS;
        L[8]=Axis(2)*Axis(3)*C1+C;
    }

    void TranslateToMatrix4(float* L)
    {
        float   C=cos(Angel),
                S=sin(Angel),
                C1=1-C,
                XY=Axis(0)*Axis(1),
                XZ=Axis(0)*Axis(2),
                YZ=Axis(1)*Axis(2),
                XS=Axis(0)*S,
                YS=Axis(1)*S,
                ZS=Axis(2)*S;
        L[0]=Axis(0)*Axis(0)*C1+C;
        L[1]=XY*C1+ZS;
        L[2]=XZ*C1-YS;
        L[3]=0.0;
        L[4]=XY*C1-ZS;
        L[5]=Axis(1)*Axis(1)*C1+C;
        L[6]=YZ*C1+XS;
        L[7]=0.0;
        L[8]=XZ*C1+YS;
        L[9]=YZ*C1-XS;
        L[10]=Axis(2)*Axis(3)*C1+C;
        L[11]=0.0;
        L[12]=0.0;
        L[13]=0.0;
        L[14]=0.0;
        L[15]=1.0;
    }
};

struct Rotate:Matrix<float,3,3>
{
    void SelfUnit()
    {
        Normal<3> T[3];
        for(int i=0;i<2;++i)
        {
            for(int k=0;k<3;++k)
                T[i](k)=this->operator()(i,k);
            T[i].SelfUnit();
        }
        T[2]=T[0]%T[1];
        for(int i=0;i<3;++i)
        {
            for(int k=0;k<3;++k)
                this->operator()(i,k)=T[i](k);
        }
    }

    /*Vertex<3> operator* (const Vertex<3>& P)
    {
        return Matrix<float,3,3>::operator*(P);
    }

    Normal<3> operator* (const Normal<3>& P)
    {
        return Matrix<float,3,3>::operator*(P);
    }
};*/

struct Coordiante
{
    //Rotate  R;
    Normal<3>   Move;
    /*Vertex<3> operator* (const Vertex<3>& P)
    {
        return R*P+Move;
    }
    Normal<3> operator* (const Normal<3>& L)
    {
        return R*L;
    }*/
};

#endif // GLENTITY_HPP_INCLUDED
