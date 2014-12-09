#include <gl/gl.h>
#include "figureset.h"
namespace PO
{
    namespace VerTool
    {
        void Translation(VertexData& VI,float X,float Y,float Z)
        {
            if(!VI.Vertex.empty())
            {
                if(VI.VerDim==2)
                {
                    size_t Tem=VI.Vertex.size()/2;
                    for(size_t i=0;i<Tem;++i)
                    {
                        VI.Vertex[i*2]+=X;
                        VI.Vertex[i*2+1]+=Y;
                    }
                }else if(VI.VerDim==3)
                {
                    size_t Tem=VI.Vertex.size()/3;
                    for(size_t i=0;i<Tem;++i)
                    {
                        VI.Vertex[i*3]+=X;
                        VI.Vertex[i*3+1]+=Y;
                        VI.Vertex[i*3+2]+=Z;
                    }
                }
            }
        }

        void SetRect(VertexData& VI,float SX,float SY,float WX,float WY,float HX,float HY)
        {
            VI.VerDim=2;
            VI.Vertex.resize(8);
            VI.Order.resize(6);
            VI.Type=GL_TRIANGLES;
            VI.MaxOrder=4;
            VI.Vertex[0]=SX;VI.Vertex[1]=SY;VI.Vertex[2]=SX+WX;VI.Vertex[3]=SY+WY;
            VI.Vertex[4]=SX+WX+HX;VI.Vertex[5]=SY+WY+HY;VI.Vertex[6]=SX+HX;VI.Vertex[7]=SY+HY;
            VI.Order[0]=0;VI.Order[1]=1;VI.Order[2]=2;
            VI.Order[3]=0;VI.Order[4]=2;VI.Order[5]=3;
        }
        void SetLine(VertexData& VI,float SX,float SY,float EX,float EY)
        {
            VI.VerDim=2;
            VI.Vertex.resize(4);
            VI.Order.resize(2);
            VI.Type=GL_LINES;
            VI.MaxOrder=2;
            VI.Vertex[0]=SX;VI.Vertex[1]=SY;VI.Vertex[2]=EX;VI.Vertex[3]=EY;
            VI.Order[0]=0;VI.Order[1]=1;
        }
        void AddLine(VertexData& VI,float SX,float SY,float EX,float EY)
        {
            if(VI.Type==GL_LINES&&VI.VerDim==2)
            {
                size_t i=VI.Vertex.size();
                VI.Vertex.resize( i+4 );
                VI.Vertex[i]=SX;VI.Vertex[i+1]=SY;VI.Vertex[i+2]=EX;VI.Vertex[i+3]=EY;
                size_t Y=VI.Order.size();
                VI.Order.resize(Y+2);
                VI.Order[Y]=i/2;VI.Order[Y+1]=i/2+1;
            }else SetLine(VI,SX,SY,EX,EY);
        }

        void SetLineLoop(VertexData& VI,float SX,float SY,float EX,float EY)
        {
            VI.VerDim=2;
            VI.Vertex.resize(4);
            VI.Order.resize(2);
            VI.Type=GL_LINE_LOOP;
            VI.MaxOrder=2;
            VI.Vertex[0]=SX;VI.Vertex[1]=SY;VI.Vertex[2]=EX;VI.Vertex[3]=EY;
            VI.Order[0]=0;VI.Order[1]=1;
        }

        void AddLineLoop(VertexData& VI,float X,float Y)
        {
            if(VI.Type==GL_LINE_LOOP&&VI.VerDim==2)
            {
                size_t i=VI.Vertex.size();
                VI.Vertex.resize( i+2 );
                VI.Vertex[i]=X;VI.Vertex[i+1]=Y;
                VI.Order.push_back(i/2);
            }
        }

        void Paint(VertexData& VI,float R,float G,float B)
        {
            size_t Io=VI.Size();
            VI.ColDim=3;
            VI.Color.resize(Io*3);
            for(size_t i=0;i<Io;++i)
                VI.Color[i*3]=R;
            for(size_t i=0;i<Io;++i)
                VI.Color[i*3+1]=G;
            for(size_t i=0;i<Io;++i)
                VI.Color[i*3+1]=B;
        }
        void Paint(VertexData& VI,float R,float G,float B,float A)
        {
            size_t Io=VI.Size();
            VI.ColDim=4;
            VI.Color.resize(Io*4);
            for(size_t i=0;i<Io;++i)
                VI.Color[i*4]=R;
            for(size_t i=0;i<Io;++i)
                VI.Color[i*4+1]=G;
            for(size_t i=0;i<Io;++i)
                VI.Color[i*4+2]=B;
            for(size_t i=0;i<Io;++i)
                VI.Color[i*4+3]=A;
        }
        void Cover(VertexData& CI,float A)
        {
            if(CI.ColDim==4)
            {
                size_t Si=CI.Color.size()/4;
                if(Si>0)
                {
                    for(size_t i=0;i<Si;++i)
                        CI.Color[i*4+3]=A;
                }
            }
        }
    }
    namespace NorTool
    {
        //void EqualNormal(VertexInfo&,ColorInfo&,float X,float Y,float Z){}
    }
    namespace TexTool
    {
        void VerRotEqu2D(VertexData& VI,TextureData& TI,float Rotate,float SX,float SY)
        {
            if(TI&&VI.VerDim==2)
            {
                size_t Size=VI.Size();
                if(Size>0)
                {
                    TI.TexCoord.resize(VI.Vertex.size());
                    float XC=VI.Vertex[0],YC=VI.Vertex[1];
                    TI.TexCoord[0]=SX;TI.TexCoord[1]=SY;
                    for(size_t i=1;i<Size;++i)
                    {
                        TI.TexCoord[i*2]=VI.Vertex[i*2]-XC+SX;
                        TI.TexCoord[i*2+1]=VI.Vertex[i*2+1]-YC+SY;
                    }
                }
            }else{
                TI.TexCoord.clear();
            }
        }

        void TexRotEqu(VertexData& VI,TextureData& TI,float SX,float SY,float Size)
        {
            if(TI)
            {
                TI.TexCoord.resize(8);
                TI.TexCoord[0]=0.0;TI.TexCoord[1]=0.0;TI.TexCoord[2]=TI.GetRateW();TI.TexCoord[3]=0.0;
                TI.TexCoord[4]=TI.GetRateW();TI.TexCoord[5]=TI.GetRateH();TI.TexCoord[6]=0.0;TI.TexCoord[7]=TI.GetRateH();
                VerTool::SetRect(VI,SX-Size*TI.GetRateWH()/2.0,SY+Size/2.0,Size*TI.GetRateWH(),0.0,0.0,-Size);
            }else{
                TI.TexCoord.clear();
                VI.clear();
            }
        }
    }
}
