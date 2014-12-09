#ifndef POTEXT_HPP
#define POTEXT_HPP
#include <sstream>
#include <deque>
#include <vector>
#include "pofont.h"


namespace PO
{
    struct DynamicDrawData
    {
        VertexInfo<> Ver;
        std::vector<float> TexCoord;
    };
    class StaticText:public VertexInfo<>
    {
        std::shared_ptr<StaticTextData> Data;
    public:
        operator bool() const {return Data&&*Data;}
        operator TextureData& () {return static_cast<TextureData&>(*Data);}
        operator StaticTextData& () {return (*Data);}
        int GetWidth() const {return Data->GetWidth();}
        StaticText(){}
        void DrawText(Render& R)
        {
            if(Data&&*Data)
            {
                auto K=this->GetCheck();
                TextureData& TD(*Data);
                TD.OpenAttitude(R,K);
                Draw(R);
                TD.CloseAttitude(R);
            }
        }
        void CreateStaticText(FontManager& FM,const std::string& L){Data=FM.CreateStaticText(L);}
        void CreateStaticText(FontManager& FM,const std::wstring& L){Data=FM.CreateStaticText(L);}
        void Free(){Data.reset();}
    };

    struct SingleCharPattern
    {
        virtual void operator()(Render&,DynamicDrawData&,const SingleCharStruct&)=0;
    };

    template<template<typename T,typename ...OT> class T> class DynamicTextBase;
    template<template<typename N,typename ...AL> class Tank>
    struct WholeStringPattern
    {
        virtual void operator()(Render&,DynamicTextBase<Tank>&)=0;
    };

    template<template<typename T,typename ...OT> class T>
    class DynamicTextBase:public DynamicTextData<T>
    {
        DynamicDrawData DDD;
    public:
        void CreateDynamic(FontManager& FM,const std::string& L){FM.CreateDynamicText(L,*this);}
        void CreateDynamic(FontManager& FM,const std::wstring& L){FM.CreateDynamicText(L,*this);}
        void AddDynamic(FontManager& FM,const std::string& L){FM.AddDynamicText(L,*this);}
        void AddDynamic(FontManager& FM,const std::wstring& L){FM.AddDynamicText(L,*this);}
        template<typename ...ALL>
            void DrawDynamic( Render&,SingleCharPattern&&);
        template<typename ...ALL>
            void DrawDynamic( Render& R,WholeStringPattern<T>&& WSP){  WSP(R,*this);    }
    };

    class DynamicText:public DynamicTextBase<std::deque>{};

    class NumberText:public DynamicTextBase<std::vector>
    {
        static std::wstringstream WSS;
    public:
        template<typename Type>
            void Connect( FontManager& FM,const Type& T )
            {
                WSS.str();
                WSS.clear();
                WSS<<T;
                std::wstring Tem;
                WSS>>Tem;
                FM.CreateDynamicText(Tem,*this);
            }
    };

    template<template<typename ,typename ...> class Tan> template<typename ...ALL>
        void DynamicTextBase<Tan>::DrawDynamic( Render& R,SingleCharPattern&& SCP)
        {
            GraphicAttitude& GA(R);
            int RecordWidth=0;
            for(auto &SCD :*this)
            {
                if(!SCD) { RecordWidth++;continue;  }
                SCP(R,DDD,*SCD);
                auto K=DDD.Ver.GetCheck();
                if(K.Size()==DDD.TexCoord.size()/2)
                {
                    GA.OpenTexCooArr(DDD.TexCoord);
                    GA.OpenTex2D(SCD->GetIndex());
                    DDD.Ver.DrawWithOutClose(GA);
                }else{
                    GA.CloseTexCooArr();
                    GA.CloseTex2D();
                }
                RecordWidth+=SCD->GetWidth();
            }
            GA.CloseTex2D();
            GA.CloseTexCooArr();
            DDD.Ver.DrawClose(GA);
        }

    namespace DynTool
    {
        struct Nor:public SingleCharPattern
        {
            float Size,NowDrawingX,NowDrawingY,ColorR,ColorG,ColorB,ColorA;
            bool Ready;
            Nor(float size,float NX,float NY,float R=1.0,float G=1.0,float B=1.0,float A=1.0);
            void operator()(Render&,DynamicDrawData&,const SingleCharStruct&);
        };
        struct Nor_LimX:public SingleCharPattern
        {
            float Size,NowDrawingX,NowDrawingY,ColorR,ColorG,ColorB,ColorA,StartDrawingX,LimitedX;
            bool Ready;
            Nor_LimX(float size,float NX,float NY,float LX,float R=1.0,float G=1.0,float B=1.0,float A=1.0);
            void operator()(Render&,DynamicDrawData&,const SingleCharStruct&);
        };
        //void Normal(DynamicDrawData&,const std::shared_ptr<SingleCharStruct>& Tem,float S,float SX,float SY);
        //void Normal_LimX(DynamicDrawData&,const std::shared_ptr<SingleCharStruct>& Tem,float S,float SX,float SY,float LimitedX);
        //void Normal_LimX_BacGro(DynamicDrawData&,const std::shared_ptr<SingleCharStruct>& Tem,float S,float SX,float SY,float LimitedX,float R,float G,float B);
        //void Normal_C(DynamicDrawData&,const std::shared_ptr<SingleCharStruct>& Tem,float S,float SX,float SY,float R,float G,float B,float A);
    }

        //void SetRectangle2D(StaticText& ST,float Size,const Vertex<2>& Sh=Ver(0.0,0.0),const Color<4>& Co=Col(1.0,1.0,1.0,1.0)){SetEqualRectangle(ST,ST,Size,Sh,Co);}
        //void Translation( StaticText& ST,float Size,const Vertex<2>& Sh ){ Translation(ST.STP.GTP,Sh); }

    /*struct DynamicStruct:public TexturePattern
    {
        unsigned int Count=0;
        const Font::DynamicTextData* DTD;
        float GetRateW(){return DTD->GetRateW();}
        float GetRateH(){return DTD->GetRateH();}
        unsigned int GetWidth(){return DTD->GetWidth();}
        unsigned int GetPoint(){return DTD->GetPoint();}
    };

    class TextDynamic
    {
    protected:
        Font::DynamicText<std::deque> PA;
    public:
        void Clear(){PA.clear();}
        bool CreateText(const std::string& P){Clear();return Font::Manager::CreateDynamicText(P,PA);}
        bool InsertText(const std::string& P){return Font::Manager::CreateDynamicText(P,PA);}
        operator bool(){return !PA.empty();}
        unsigned int Width();
        unsigned int PopFront(unsigned int S=1);
        unsigned int PopBack(unsigned int S=1);
        template<typename Type,typename ...TP>
            void Display(const Type& L,const TP&... TO) const
            {
                DynamicStruct Tem;
                glEnableClientState(GL_VERTEX_ARRAY);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glEnableClientState(GL_COLOR_ARRAY);
                glEnableClientState(GL_NORMAL_ARRAY);
                glEnable(GL_TEXTURE_2D);
                for( auto Pl:PA)
                {
                    if( Font::Manager::GetDynamicData(Pl,Tem.DTD))
                    {
                        L(Tem,TO...);
                        if(Tem.Legal())
                        {
                            glVertexPointer(Tem.LS,GL_FLOAT,0,&Tem.Loc[0]);
                            glTexCoordPointer(2,GL_FLOAT,0,&Tem.Tex[0]);
                            glColorPointer(Tem.CS,GL_FLOAT,0,&Tem.Col[0]);
                            glNormalPointer(GL_FLOAT,0,&Tem.Nor[0]);
                            glBindTexture(GL_TEXTURE_2D,Tem.GetPoint());
                            glDrawElements(GL_TRIANGLES,Tem.Dra.size(),GL_UNSIGNED_INT,&Tem.Dra[0]);
                            Tem.Count+=(Tem.DTD)->GetWidth();
                        }else Tem.Count+=1;
                    }else{
                        Tem.Count+=1;
                    }
                }
                glDisableClientState(GL_VERTEX_ARRAY);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                glDisableClientState(GL_COLOR_ARRAY);
                glDisableClientState(GL_NORMAL_ARRAY);
                glDisable(GL_TEXTURE_2D);
            }
    };

    unsigned int TextDynamic::PopFront(unsigned int S)
    {
        unsigned int Size=PA.size();
        if(Size<=S)
        {
            PA.clear();
            return Size;
        }else{
            for(int i=0;i<S;++i)
                PA.pop_front();
            return S;
        }
    }

    unsigned int TextDynamic::PopBack(unsigned int S)
    {
        unsigned int Size=PA.size();
        if(Size<=S)
        {
            PA.clear();
            return Size;
        }else{
            for(int i=0;i<S;++i)
                PA.pop_back();
            return S;
        }
    };

    class TextNumber
    {
        static std::stringstream StaticStream;
        TextDynamic TD;
        std::string Number;
    public:
        template<typename ST,typename T,typename ...TP>
            void Display(const ST& Ty,const T& L,const TP&... TO)
            {
                StaticStream.clear();
                StaticStream<<Ty;
                StaticStream>>Number;
                TD.CreateText(Number);
                TD.Display(L,TO...);
            }
    };
    std::stringstream TextNumber::StaticStream;

    unsigned int TextDynamic::Width()
    {
        unsigned int CW=0;
        unsigned int Point;
        const Font::DynamicTextData* DTD;
        for(auto Po=PA.begin();Po!=PA.end();++Po)
            if(Font::Manager::GetDynamicData(*Po,DTD))
                CW+=DTD->GetWidth();
        return CW;
    }


    auto DynTexSqu_P2SC4=std::bind(
                                    [] (DynamicStruct& DS , const Point<2>& S , float Size ,const Color<4>& C)
                                  {
                                        if(DS.Count==0)
                                        {
                                            DS.LPS=2;
                                            DS.LS=2;
                                            DS.CS=4;
                                            DS.Dra={0,1,2,2,0,3};
                                            DS.Col ={     C(0),C(1),C(2),C(3),
                                                                C(0),C(1),C(2),C(3),
                                                                C(0),C(1),C(2),C(3),
                                                                C(0),C(1),C(2),C(3)};

                                            DS.Nor={     0.0,0.0,-1.0,
                                                             0.0,0.0,-1.0,
                                                              0.0,0.0,-1.0,
                                                               0.0,0.0,-1.0};
                                        }
                                        DS.Tex={0.0,0.0,
                                                    DS.GetRateW(),0.0,
                                                    DS.GetRateW(),DS.GetRateH(),
                                                    0.0,DS.GetRateH() };
                                        float TemSize=DS.Count*0.25*Size;
                                        float EachSize=DS.GetWidth()*0.5;
                                        float SX=S(0)+DS.Count*0.25*Size;
                                        float EaX=DS.GetWidth()*0.25*Size;
                                        DS.Loc={   SX,S(1),
                                                        SX+EaX,S(1),
                                                        SX+EaX,S(1)-2*EaX,
                                                        SX,S(1)-2*EaX};
                                  }
                                  ,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);*/
}
#endif // POTTF_H
