#ifndef FIGURESET_H_INCLUDED
#define FIGURESET_H_INCLUDED
#include <vector>
#include "graphic.h"
#include "../templatemetaprogramming/adapter.h"
#include "../templatemetaprogramming/inheritance.h"
namespace PO
{
    struct InfoCheck
    {
        size_t VertexSize;
        size_t Size() const {return VertexSize;}
        InfoCheck(size_t K):VertexSize(K){}
    };

    struct VertexData
    {
        GLenum Type=GL_POINTS;
        std::vector<unsigned int> Order;
        size_t VerDim=2;
        std::vector<float> Vertex;
        unsigned int MaxOrder=0;

        size_t ColDim=4;
        std::vector<float> Color;

        bool SizeEqual() const {return Size()==Color.size()/ColDim;}
        bool Avalible() const {return Size()>=MaxOrder;}
        size_t Size() const {return Vertex.size()/VerDim;}
        InfoCheck GetCheck() const {  InfoCheck Tem(Size());return Tem;}
        void OpenAttitude(GraphicAttitude& GA,const InfoCheck& IC)
        {
            if(Avalible())
            {
                GA.OpenVerArr(Vertex,VerDim);
                if(SizeEqual())
                    GA.OpenColArr(Color,ColDim);
                else
                    GA.CloseColArr();
            }else{
                CloseAttitude(GA);
            }
        }
        void CloseAttitude(GraphicAttitude& GA)
        {
            GA.CloseVerArr();
            GA.CloseColArr();
        }
        void clear()
        {
            Type=GL_POINTS;
            Order.clear();
            Vertex.clear();
            MaxOrder=0;
            Color.clear();
        }
    };

    struct NormalData
    {
        std::vector<float> Normal;
        bool operator ==( const InfoCheck& DI ) const {return Size()==DI.Size();}
        size_t Size() const {return Normal.size()/3; }
        void OpenAttitude(GraphicAttitude& GA,const InfoCheck& IC)
        {
            if(*this==IC)
                GA.OpenNorArr(Normal);
            else
                GA.CloseNorArr();
        }
        void CloseAttitude(GraphicAttitude& GA)
        {
            GA.CloseNorArr();
        }
    };

    class TextureData
    {
        std::shared_ptr<GraphicTexture> Data;
        void StartEnviron();
        void EndEnviron();
    public:
        std::vector<float> TexCoord;

        size_t Size() const {return TexCoord.size()/2; }
        bool operator == ( const InfoCheck& DI) const { return Size()==DI.Size(); }
        void ForceDelete(){decltype(Data) Tem;Data=Tem;}
        operator bool () const {  return static_cast<bool>(Data);  }
        TextureData(){}
        TextureData(const std::shared_ptr<GraphicPreTexture>& SH):Data( new GraphicTexture(*SH)){}
        TextureData(const GraphicPreTexture& GTD):Data(new GraphicTexture(GTD)){}
        void Swap( std::shared_ptr<GraphicTexture>&& GT  ){  Data.swap(GT); }
        void Swap( std::shared_ptr<GraphicTexture>& GT  ){  Data.swap(GT); }
        float GetRateWH(){return Data->GetRateWH();}
        float GetRateW(){return Data->GetRateW();}
        float GetRateH(){return Data->GetRateH();}
        unsigned int GetIndex()const {return Data->GetPoint();}
        void OpenAttitude(GraphicAttitude& GA,const InfoCheck& IC) const
        {
            if(*this==IC)
            {
                GA.OpenTexCooArr(TexCoord);
                GA.OpenTex2D(GetIndex());
            }else{
                CloseAttitude(GA);
            }
        }
        void CloseAttitude(GraphicAttitude& GA) const
        {
            GA.CloseTex2D();
            GA.CloseNorArr();
        }
    };

    struct EmptyData
    {
        void OpenAttitude(GraphicAttitude& GA,const InfoCheck& IC){}
        void CloseAttitude(GraphicAttitude& GA){}
    };

    template<typename T=EmptyData,typename ...OT>
    struct VertexInfo:public UnbalanceTreeInheritance<VertexData,T,OT...>
    {
        template<typename Ti> struct OpenAttitudeType
        {
            static void Run(Ti& Tc,GraphicAttitude& GA,const InfoCheck& IC){Tc.OpenAttitude(GA,IC);}
        };
        template<typename Ti> struct CloseAttitudeType
        {
            static void Run(Ti& Tc,GraphicAttitude& GA){Tc.CloseAttitude(GA);}
        };
    public:
        void Draw(GraphicAttitude& GA)
        {
            VertexData& VD(*this);
            if(VD.Avalible())
            {
                InfoCheck IC=VertexData::GetCheck();
                UnbalanceTreeInheritance<VertexData,T,OT...>& Temm(*this);
                UnbalanceTreeRun<OpenAttitudeType>::FindPosRun(Temm,GA,IC);
                glDrawElements(VD.Type,VD.Order.size(),GL_UNSIGNED_INT,&VD.Order[0]);
                UnbalanceTreeRun<CloseAttitudeType>::FindPreRun(Temm,GA);
            }
        }
        void DrawWithOutClose(GraphicAttitude& GA)
        {
            VertexData& VD(*this);
            if(VD.Avalible())
            {
                InfoCheck IC=VertexData::GetCheck();
                UnbalanceTreeInheritance<VertexData,T,OT...>& Temm(*this);
                UnbalanceTreeRun<OpenAttitudeType>::FindPosRun(Temm,GA,IC);
                glDrawElements(VD.Type,VD.Order.size(),GL_UNSIGNED_INT,&VD.Order[0]);
            }
        }
        void DrawClose(GraphicAttitude& GA)
        {
            VertexData& VD(*this);
            UnbalanceTreeInheritance<VertexData,T,OT...>& Temm(*this);
            if(VD.Avalible())
                UnbalanceTreeRun<CloseAttitudeType>::FindPreRun(Temm,GA);
        }
    };

    namespace VerTool
    {
        void SetRect(VertexData&,float SX,float SY,float WX,float WY,float HX,float HY);
        void SetLine(VertexData&,float SX,float SY,float EX,float EY);
        void AddLine(VertexData&,float SX,float SY,float EX,float EY);
        void SetLineLoop(VertexData&,float SX,float SY,float EX,float EY);
        void AddLineLoop(VertexData&,float X,float Y);

        void Translation(VertexData& VI,float X,float Y,float Z);
        void Paint(VertexData&,float R=1.0,float G=1.0,float B=1.0);
        void Paint(VertexData&,float R,float G,float B,float A);
        void Cover(VertexData&,float A);
    }

    namespace NorTool
    {
        //inline void EqualNormal(VertexInfo&,NormalInfo&,float X,float Y,float Z){}
    }

    namespace TexTool
    {
        void VerRotEqu2D(VertexData& VI,TextureData& TI,float Rotate,float SX=0.0,float SY=1.0);
        inline void VerRotEqu2D(Adapter<VertexData,TextureData> TI,float Rotate,float SX=0.0,float SY=1.0){VerRotEqu2D(TI,TI,Rotate,SX,SY);}
        void TexRotEqu(VertexData& VI,TextureData& TI,float SX,float SY,float Size=1.0);
        inline void TexRotEqu(Adapter<VertexData,TextureData> TI,float SX,float SY,float Size=1.0){TexRotEqu(TI,TI,SX,SY,Size);}
    }
}
#endif // GLPATTERN_HPP_INCLUDED
