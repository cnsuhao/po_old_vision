#include "elfpicture.h"
namespace PO{
        void ElfPicture::LoadElf(ImageManager& IM,const std::string& L,int W,int H,float Size)
        {
            Create(IM,L);
            WC=W;HC=H;
            ElfTool::ElfSetRect(*this,0.0,0.0,Size);
            VerTool::Paint(*this,1.0,1.0,1.0,1.0);
        }
namespace ElfTool
{
    void SetState(ElfPicture& EP,int W,int H)
    {
        float TX=(EP.IS)->GetRateW();
        float TY=(EP.IS)->GetRateH();

        float RTC1=static_cast<float>(W)/EP.WC*TX;
        float RTC2=static_cast<float>(W+1)/EP.WC*TX;
        (EP.IS)->TexCoord.resize(8);
        (EP.IS)->TexCoord[0]=RTC1;(EP.IS)->TexCoord[6]=RTC1;
        (EP.IS)->TexCoord[2]=RTC2;(EP.IS)->TexCoord[4]=RTC2;
        RTC1=static_cast<float>(H)/EP.HC*TY;
        RTC2=static_cast<float>(H+1)/EP.HC*TY;
        (EP.IS)->TexCoord[1]=(EP.IS)->TexCoord[3]=RTC1;
        (EP.IS)->TexCoord[5]=(EP.IS)->TexCoord[7]=RTC2;
    }
    void ElfSetRect(ElfPicture& TRE,float SX,float SY,float Size)
    {
        VerTool::SetRect(TRE,SX-TRE.GetRateWH()*Size/TRE.WC*TRE.HC/2.0,SY+Size/2.0,TRE.GetRateWH()*Size/TRE.WC*TRE.HC,0.0,0.0,-Size);
        PO::ElfTool::SetState(TRE,0,0);
    }
}}
/*void Pattern::Elv::SetEqualRectangle(ElfPicture& EP,float Size,const Vertex<2>& Shift,const Color<4>& C,int W,int H)
{
    float TX=(EP.P.IS)->GetGraphic()->GetRateW();
    float TY=(EP.P.IS)->GetGraphic()->GetRateH();

    float RT=(EP.P.IS)->GetGraphic()->GetRateWH()*Size;
    Pattern::Fig::AddRectance2D(EP.P.PD.GLP,Shift+Nor(-Size/2,RT/2),Nor(Size,0.0),Nor(0.0,-RT),C);

    float RTC1=static_cast<float>(W)/EP.WC*TX;
    float RTC2=static_cast<float>(W+1)/EP.WC*TX;
    EP.P.PD.TextureCoor.resize(8);
    EP.P.PD.TextureCoor[0]=RTC1;EP.P.PD.TextureCoor[6]=RTC1;
    EP.P.PD.TextureCoor[2]=RTC2;EP.P.PD.TextureCoor[4]=RTC2;
    RTC1=static_cast<float>(H)/EP.HC*TY;
    RTC2=static_cast<float>(H+1)/EP.HC*TY;
    EP.P.PD.TextureCoor[1]=EP.P.PD.TextureCoor[3]=RTC1;
    EP.P.PD.TextureCoor[5]=EP.P.PD.TextureCoor[7]=RTC2;
}

void Pattern::Elv::SetState(ElfPicture& EP,int W,int H)
{
    float TX=(EP.P.IS)->GetGraphic()->GetRateW();
    float TY=(EP.P.IS)->GetGraphic()->GetRateH();
    float RTC1=static_cast<float>(W)/EP.WC*TX;
    float RTC2=static_cast<float>(W+1)/EP.WC*TX;
    EP.P.PD.TextureCoor.resize(8);
    EP.P.PD.TextureCoor[0]=RTC1;EP.P.PD.TextureCoor[6]=RTC1;
    EP.P.PD.TextureCoor[2]=RTC2;EP.P.PD.TextureCoor[4]=RTC2;
    RTC1=static_cast<float>(H)/EP.HC*TY;
    RTC2=static_cast<float>(H+1)/EP.HC*TY;
    EP.P.PD.TextureCoor[1]=EP.P.PD.TextureCoor[3]=RTC1;
    EP.P.PD.TextureCoor[5]=EP.P.PD.TextureCoor[7]=RTC2;
}*/
