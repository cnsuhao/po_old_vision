#ifndef KEYINPIUT_H_INCLUDED
#define KEYINPIUT_H_INCLUDED
#include "potext.h"
#include <list>
#include <string>
namespace PO
{

    struct KeyInputSingleDisplay
    {
    public:
        virtual void DrawCursor(Render&)=0;
        virtual void operator() (Render&,DynamicDrawData& ,const SingleCharStruct&)=0;
    };

    struct KeyInputWholeDisplay
    {
        virtual void operator() (Render&,const DynamicTextBase<std::list>&,const DynamicTextBase<std::list>::iterator&)=0;
    };

    class KeyInput
    {
        DynamicDrawData DDD;
        DynamicTextBase<std::list> DT;
        DynamicTextBase<std::list>::iterator Cursor;
        bool Respond=false;
        bool TextReady=false;
        std::string TexInput;
        long CursorTime;
        long InputTime;
        wchar_t InputKey;
        void ResetDynamic();
        void ResetState();
    public:
        typedef DynamicTextBase<std::list> DataType;
        KeyInput();
        bool IsRespond() const {return Respond;}
        bool RespondEvent(const EventBox&,Adapter<FontManager,Render> AD);
        bool IsInputReady() const {return TextReady;}
        std::wstring GetText();
        void StartInput(int x,int y,int w,int h);
        void StartInput(const Render&,float X,float Y,float W,float H);
        void StopInput();
        void DrawInput(Render&,KeyInputSingleDisplay&&);
        void DrawInput(Render&,KeyInputWholeDisplay&&);
    };

    /*template<typename ...AP>
        void KeyInput::Displaye(Render& AD,void (*FP)(DynamicDrawData&,const std::shared_ptr<SingleCharStruct>& Tem,AP... ),AP... ap)
        {
            int RecordWidth=0;
            for(auto &SCD :*this)
            {
                DDD.Rendered=RecordWidth;
                if(!SCD) { RecordWidth++;continue;  }
                (*FP)(DDD,SCD,al...);
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
        }*/

    namespace KeyTool
    {
        struct Nor:public DynTool::Nor_LimX,public KeyInputSingleDisplay
        {
            VertexInfo<> Cursor;
            Nor(float s,float x,float y,float lx,float r=1.0,float g=1.0,float b=1.0,float a=1.0):Nor_LimX(s,x,y,lx,r,g,b,a){}
            void operator() (Render&,DynamicDrawData& ,const SingleCharStruct&);
            void DrawCursor(Render&);
        };
    }
}


#endif // INPIUT_H_INCLUDED
