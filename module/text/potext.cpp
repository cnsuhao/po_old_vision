#include "potext.h"
using namespace std;
#include <iostream>
using namespace std;
namespace PO
{
    namespace DynTool
    {
        Nor::Nor(float size,float NX,float NY,float R,float G,float B,float A):Size(size),NowDrawingX(NX),
                                    NowDrawingY(NY),ColorR(R),ColorB(B),ColorA(A),ColorG(G),Ready(false)
        {
            //Just Empty....
        }
        void Nor::operator()(Render& R,DynamicDrawData& DDD,const SingleCharStruct& SCS)
        {
            auto& Ve=DDD.TexCoord;
            auto& Iu=DDD.Ver.Vertex;
            if(!Ready)
            {
                DDD.TexCoord.resize(8);
                VerTool::SetRect(DDD.Ver,0.0,0.0,0.0,0.0,0.0,0.0);
                VerTool::Paint(DDD.Ver,1.0,1.0,1.0,1.0);
                Ve[0]=Ve[6]=Ve[1]=Ve[3]=0;
                Iu[1]=Iu[3]=NowDrawingY+Size/2.0;
                Iu[7]=Iu[5]=NowDrawingY-Size/2.0;
                Ready=true;
            }
            Ve[2]=Ve[4]=SCS.GetRateW();
            Ve[7]=Ve[5]=SCS.GetRateH();
            Iu[0]=Iu[6]=NowDrawingX;
            Iu[2]=Iu[4]=NowDrawingX+SCS.GetWidth()*Size*0.5;
            NowDrawingX+=SCS.GetWidth()*Size*0.5;
        }
        Nor_LimX::Nor_LimX(float size,float NX,float NY,float LX,float R,float G,float B,float A):Size(size),NowDrawingX(NX),
                                    NowDrawingY(NY),ColorR(R),ColorB(B),ColorA(A),ColorG(G),Ready(false),
                                    StartDrawingX(NX),LimitedX(LX)
        {
            //Just Empty....
        }
        void Nor_LimX::operator()(Render& R,DynamicDrawData& DDD,const SingleCharStruct& SCS)
        {
            auto& Ve=DDD.TexCoord;
            auto& Iu=DDD.Ver.Vertex;
            if(!Ready)
            {
                DDD.TexCoord.resize(8);
                VerTool::SetRect(DDD.Ver,0.0,0.0,0.0,0.0,0.0,0.0);
                VerTool::Paint(DDD.Ver,1.0,1.0,1.0,1.0);
                Ve[0]=Ve[6]=Ve[1]=Ve[3]=0;
                Iu[1]=Iu[3]=NowDrawingY+Size/2.0;
                Iu[7]=Iu[5]=NowDrawingY-Size/2.0;
                Ready=true;
            }
            if(NowDrawingX+SCS.GetWidth()*Size*0.5>LimitedX)
            {
                NowDrawingX=StartDrawingX;
                Iu[1]=Iu[3]=NowDrawingY-Size/2.0;
                NowDrawingY-=Size;
                Iu[7]=Iu[5]=NowDrawingY-Size/2.0;
            }
            Ve[2]=Ve[4]=SCS.GetRateW();
            Ve[7]=Ve[5]=SCS.GetRateH();
            Iu[0]=Iu[6]=NowDrawingX;
            Iu[2]=Iu[4]=NowDrawingX+SCS.GetWidth()*Size*0.5;
            NowDrawingX+=SCS.GetWidth()*Size*0.5;
        }
    }
    std::wstringstream NumberText::WSS;
}
