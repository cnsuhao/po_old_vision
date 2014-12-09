#ifndef DIALOG_HPP
#define DIALOG_HPP
#include "potext.hpp"
#include "language.hpp"
#include "utf8string.hpp"
#include "poevent.hpp"
namespace PO
{
    class StaticDialog
    {
        unsigned int LimitedWidth;
        std::deque<Handle<TextStatic> > AllData;
    public:
        void Insert(const std::string& J);
        unsigned int Line(){return AllData.size();}
        StaticDialog(unsigned int W):LimitedWidth(W){}
        void Display(float SX,float EX,float SY,float R=1.0, float B=1.0, float G=1.0, float A=1.0);
        void Clear(){AllData.clear();}
        unsigned int CountLine(){return AllData.size();}
        void PopFront(){if(!AllData.empty()) AllData.pop_front();}
        void PopBack(){if(!AllData.empty()) AllData.pop_back();}
    };

    class DynamicDialog:public TextDynamic
    {
    public:
        void Display(float SX,float EX,float SY,unsigned int LW,float R=1.0, float B=1.0, float G=1.0, float A=1.0);
        bool Insert(const std::string& D) {return TextDynamic::InsertText(D);}
        unsigned int CountLine(unsigned int P);
    };
    class EditDialog
    {
        Font::DynamicText<std::list> Data;
        Font::DynamicText<std::list>::iterator Cursor;
        unsigned long Time;
    public:
        EditDialog():Time(0){Cursor=Data.end();}
        bool Insert(const std::string& P);
        void CursorBack();
        void CursorFront();
        void Delete();
        std::string GetString();
        void Display(float SX,float EX,float SY,unsigned int W,unsigned long Interval,float R,float G,float B,float A);
        void Clear(){Data.clear();Cursor=Data.end();}
        bool Respond(const Handle<Event>&);
    };

    bool EditDialog::Respond(const Handle<Event>& P)
    {
        if(P->IsKey()&&P->IsKeyButtonDown())
        {
            if(P->IsKeyEnter())
                return true;
            else if(P->IsKeyRight())
                CursorFront();
            else if(P->IsKeyLeft())
                CursorBack();
            else if(P->IsKeyDelete())
                Delete();
            else if(P->IsKeySeem())
            {
                std::string K;
                K+=P->GetChar();
                Insert(K);
            }
        }
        return false;
    }

    bool EditDialog::Insert(const std::string& K)
    {
        Font::DynamicText<std::list> Tem;
        bool Resoult=Font::Manager::CreatDynamicText(K,Tem);
        Data.insert(Cursor,Tem.begin(),Tem.end());
        return Resoult;
    }

    void EditDialog::CursorBack()
    {
        if(Cursor!=Data.end())
            ++Cursor;
    }

    void EditDialog::CursorFront()
    {
        if(Cursor!=Data.begin())
            --Cursor;
    }
    void EditDialog::Delete()
    {
        auto Po=Cursor;
        if(Cursor!=Data.begin())
            Data.erase(--Po);
    }

    std::string EditDialog::GetString()
    {
        std::string Tem;
        const Font::DynamicTextData* DTD;
        unsigned int UnUse;
        for(auto Po:Data)
            if(Font::Manager::GetDynamicData(Po,DTD))
                Tem+=DTD->GetSymple();
        return Tem;
    }

    unsigned int DynamicDialog::CountLine(unsigned int P)
    {
        unsigned int CL=0,CW=0;
        const Font::DynamicTextData* DTD;
        if(PA.empty())
            return 0;
        else{
            CL=1;
            for(auto Po:PA)
                if(Font::Manager::GetDynamicData(Po,DTD))
                {
                    CW+=DTD->GetWidth();
                    if(CW>P)
                    {
                        CL+=1;
                        CW=DTD->GetWidth();
                    }
                }
        }
        return CL;
    }

    void StaticDialog::Insert(const std::string& BufferT)
    {
        std::string Buffer=BufferT;
        unsigned int CW,CC;
        std::string Tem;
        while(1)
        {
            Tem=UTF8String::GetTextWidthCut(Buffer,LimitedWidth,CC,CW);
            if(CW==0)
                break;
            else
            {
                Handle<TextStatic> Han=new TextStatic;
                Han->CreatText(Tem);
                AllData.push_back(Han);
            }
        }
    }

    void StaticDialog::Display(float SX,float EX,float SY, float R, float B, float G, float A)
    {
        float P=(EX-SX)/(LimitedWidth);
        float L=P/0.4;
        for(std::deque<Handle<TextStatic> >::iterator Po=AllData.begin();Po!=AllData.end();++Po)
        {
            (*Po)->Display(XYF(SX,SY),XYF(1.0,0.0),L,R,B,G,A);
            SY-=L;
        }
    }

    void DynamicDialog::Display(float SX,float EX,float SY,unsigned int LW,float R, float B, float G, float A)
    {
        glPushAttrib(GL_ENABLE_BIT);
        glVertexPointer(2,GL_FLOAT,0,GLTexture::Location);
        glTexCoordPointer(2,GL_FLOAT,0,GLTexture::Texture);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_TEXTURE_2D);
        glColorPointer(4,GL_FLOAT,0,GLTexture::Color);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
        GLTexture::SetColor(R,G,B,A);
        float PE=(EX-SX)/LW;
        float L=PE/0.4;
        unsigned int WC=0;
        XYF P(SX,SY);
        XYF T(PE,0.0);
        XYF D(0.0,-L);
        for(auto Po=PA.begin();Po!=PA.end();++Po)
        {
            const Font::DynamicTextData* DTD;
            if(Font::Manager::GetDynamicData(*Po,DTD))
            {
                WC+=DTD->GetWidth();
                if(WC>LW)
                {
                    WC=DTD->GetWidth();
                    P.X=SX;
                    P.Y-=L;
                }
                P.OutFloat(GLTexture::Location);
                (P+D).OutFloat(GLTexture::Location+2);
                (P+D+DTD->GetWidth()*T).OutFloat(GLTexture::Location+4);
                (P+DTD->GetWidth()*T).OutFloat(GLTexture::Location+6);
                P.X+=DTD->GetWidth()*PE;
                DTD->AdjectTextureCoor(GLTexture::TextureCoor,GLTexture::Texture,4);
                glBindTexture(GL_TEXTURE_2D,DTD->GetPoint());
                glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT,GLTexture::Order);
            }else{
                P.X+=PE;
                WC+=1;
                if(WC>LW)
                {
                    WC=1;
                    P.X=SX;
                    P.Y-=L;
                }
            }
        }
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glPopAttrib();
    }

    void EditDialog::Display(float SX,float EX,float SY,unsigned int LW,unsigned long Interval,float R,float G,float B,float A)
    {
        glPushAttrib(GL_ENABLE_BIT);

        glVertexPointer(2,GL_FLOAT,0,GLTexture::Location);
        glTexCoordPointer(2,GL_FLOAT,0,GLTexture::Texture);
        glColorPointer(4,GL_FLOAT,0,GLTexture::Color);

        GLTexture::SetColor(R,G,B,A);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
        GLTexture::SetColor(R,G,B,A);
        Time+=Interval;
        float PE=(EX-SX)/LW;
        float L=PE/0.4;
        unsigned int WC=0;
        XYF P(SX,SY);
        XYF T(PE,0.0);
        XYF D(0.0,-L);
        XYF TemCur(P);
        for(auto Po=Data.begin();Po!=Data.end();++Po)
        {
            const Font::DynamicTextData* DTD;
            if(Font::Manager::GetDynamicData(*Po,DTD))
            {
                if(Po==Cursor)
                    TemCur=P;
                WC+=DTD->GetWidth();
                if(WC>LW)
                {
                    WC=DTD->GetWidth();
                    P.X=SX;
                    P.Y-=L;
                }
                P.OutFloat(GLTexture::Location);
                (P+D).OutFloat(GLTexture::Location+2);
                (P+D+DTD->GetWidth()*T).OutFloat(GLTexture::Location+4);
                (P+DTD->GetWidth()*T).OutFloat(GLTexture::Location+6);
                P.X+=DTD->GetWidth()*PE;
                DTD->AdjectTextureCoor(GLTexture::TextureCoor,GLTexture::Texture,4);
                glBindTexture(GL_TEXTURE_2D,DTD->GetPoint());
                glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT,GLTexture::Order);
            }else{
                if(Po==Cursor)
                    TemCur=P;
                P.X+=PE;
                WC+=1;
                if(WC>LW)
                {
                    WC=1;
                    P.X=SX;
                    P.Y-=L;
                }
            }
        }
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glPopAttrib();
        if(Time<600)
        {
            if(Cursor==Data.end())
                TemCur=P;
            TemCur.OutFloat(GLTexture::Location);
            (TemCur+D).OutFloat(GLTexture::Location+2);
            (TemCur+D+0.2*T).OutFloat(GLTexture::Location+4);
            (TemCur+0.2*T).OutFloat(GLTexture::Location+6);
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,GLTexture::Order);
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_COLOR_ARRAY);
        }else if(Time>1200)
            Time=0;
    }
}
#endif // DIALOG_H
