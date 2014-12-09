#include "keyinput.h"
using namespace PO;
using namespace std;
KeyInput::KeyInput()
{
    Cursor=DT.end();
}
void KeyInput::ResetDynamic()
{
    DT.clear();
    Cursor=DT.end();
}


bool KeyInput::RespondEvent(const EventBox& Sp,Adapter<FontManager,Render> AD)
{
    if((!Respond)||(!Event::IsTextActive()))
        return false;
    for(auto &I:Sp.GetKeyBox())
    {
        Event& Ev(*I);
        if(Ev.IsKeyButtonUp())
        {
            if(Ev.IsKeyLeft())
            {
                if(Cursor!=DT.begin())
                    --Cursor;
            }else if(Ev.IsKeyRight())
            {
                if(Cursor!=DT.end())
                    ++Cursor;
            }else if(Ev.IsKeyEnter())
            {
                Respond=false;
                TextReady=true;
            }else if(Ev.IsKeyEsc())
            {
                Respond=false;
                ResetDynamic();
            }else if(Ev.IsKeyDelete())
            {
                if(!DT.empty())
                {
                    if(Cursor==DT.begin())
                    {
                    }else if(Cursor==DT.end())
                    {
                        DT.pop_back();
                        Cursor=DT.end();
                    }else{
                        auto K=Cursor;
                        K--;
                        Cursor=DT.erase(K);
                    }
                }
            }
        }
    }
    for(auto &I:Sp.GetTextBox())
    {
        Event& U(*I);
        if(U.IsTextInput())
        {
            std::string Tem;
            Tem+=U.GetTextInput();
            DynamicTextData<std::list> TemC;
            static_cast<FontManager&>(AD).CreateDynamicText(Tem,TemC);
            for(auto& L:TemC)
            {
                DT.insert(Cursor,L);
            }
        }
    }
    return true;
}

std::wstring KeyInput::GetText()
{
    std::wstring TemWS;
    if(!TextReady)
        return TemWS;
    for(auto &I:DT)
        TemWS+=I->GetChar();
    TextReady=false;
    return TemWS;
}

void KeyInput::StartInput(int x,int y,int w,int h)
{
    SDL_Rect SR={x,y,w,h};
    SDL_SetTextInputRect(&SR);
    SDL_StartTextInput();
    Respond=true;
    ResetDynamic();
}

void KeyInput::StartInput(const Render& R,float X,float Y,float W,float H)
{
    float SX=R.GetScreen2DX(),SY=R.GetScreen2DY();
    int WW=R.GetWindowW(),WH=R.GetWindowH();
    int RX=static_cast<int>( (X/SX+1)*(WW/2) ),
        RY=static_cast<int>( (1-Y/SY)*(WH/2) ),
        RW=static_cast<int>( (W/SX/2.0)*WW ),
        RH=static_cast<int>( (H/SY/2.0)*WH );
    StartInput(RX,RY,RW,RH);
}
void KeyInput::StopInput()
{
    SDL_StopTextInput();
}

void KeyInput::DrawInput(Render& R,KeyInputSingleDisplay&& KSD)
{
    GraphicAttitude& GA(R);
    bool Find=false;
    for(auto I=DT.begin(); I!=DT.end(); ++I)
    {
        auto& SCD(*I);
        if(!SCD)
        {
            continue;
        }
        if(Cursor==I)
        {
            GA.CloseTex2D();
            GA.CloseTexCooArr();
            KSD.DrawCursor(R);
            Find=true;
        }
        KSD(R,DDD,*SCD);
        auto K=DDD.Ver.GetCheck();
        if(K.Size()==DDD.TexCoord.size()/2)
        {
            GA.OpenTexCooArr(DDD.TexCoord);
            GA.OpenTex2D(SCD->GetIndex());
            DDD.Ver.DrawWithOutClose(GA);
        }
        else
        {
            GA.CloseTexCooArr();
            GA.CloseTex2D();
        }

    }
    GA.CloseTex2D();
    GA.CloseTexCooArr();
    if(!Find)
        KSD.DrawCursor(R);
    DDD.Ver.DrawClose(GA);
}
namespace PO{
namespace KeyTool{
    void KeyTool::Nor::DrawCursor(Render& R)
    {
        VerTool::SetLine(Cursor,NowDrawingX,NowDrawingY-Size/2.0,NowDrawingX,NowDrawingY+Size/2.0);
        VerTool::Paint(Cursor,ColorR,ColorG,ColorB,ColorA);
        Cursor.DrawWithOutClose(R);
    }
    void KeyTool::Nor::operator()(Render& R,DynamicDrawData& DDD,const SingleCharStruct& SCS)
    {
        Nor_LimX::operator()(R,DDD,SCS);
    }
}
}
