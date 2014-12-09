#include "graphic.h"
namespace PO
{
    GraphicContext::GraphicContext(const GraphicInfo& GI) throw( CreatingGLContextFail,CreatingWindowFail   )
        :GraphicInfo(GI)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        MainSurface= SDL_CreateWindow(Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,WindowW,WindowH,Flag );
        if(MainSurface==nullptr) throw CreatingWindowFail();

        GLContext = SDL_GL_CreateContext(MainSurface);
        if(GLContext==nullptr)  { SDL_DestroyWindow(MainSurface);throw CreatingGLContextFail();}

        SDL_GL_SetSwapInterval(1);
        SDL_GetWindowSize(MainSurface,&WindowW,&WindowH);
        WindowScale=static_cast<float>(WindowW)/WindowH;
        WindowIndex=SDL_GetWindowID(MainSurface);
        StartTime=Graphic::GetTick();
        SpendTime=0;
        IntervalTime=16;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        SDL_StopTextInput();
        GraphicAttitude::Blend_En=true;
    }

    void GraphicContext::ResetWindowSize(int X,int Y)
    {
        SDL_GL_DeleteContext(GLContext);
        SDL_SetWindowSize(MainSurface,X,Y);
        SetWindow(X,Y);
        GLContext=SDL_GL_CreateContext(MainSurface);
    }

    GraphicContext::~GraphicContext()
    {
        SDL_GL_DeleteContext(GLContext);
        SDL_DestroyWindow(MainSurface);
    }

    void GraphicContext::SetCurrent()
    {
        SDL_GL_MakeCurrent(MainSurface,GLContext);
        long Now=Graphic::GetTick();
        IntervalTime=Now-StartTime-SpendTime;
        SpendTime+=IntervalTime;
    }

    void GraphicContext::SetPerspective()
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-Screen2DY*ScreenScale,Screen2DY*ScreenScale,-Screen2DY,Screen2DY,Screen3DNear,Screen3DFar);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void GraphicContext::SetParallel()
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-Screen2DY*ScreenScale,Screen2DY*ScreenScale,-Screen2DY,Screen2DY,-1.0,1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void GraphicContext::PointCalculate(int Xi,int Yi,float &R,float &P) const
    {
        R=(Xi*2.0-GetWindowW())*GetScreen2DX()/GetWindowW();
        P=(GetWindowH()-Yi*2.0)*GetScreen2DY()/GetWindowH();
    }

    bool Graphic::GetEvent(std::shared_ptr<Event>& I)
    {
        SDL_Event Eve;
        if(SDL_PollEvent(&Eve))
        {
            std::shared_ptr<Event> Tem(new Event(Eve));
            I =Tem;
            return true;
        }
        return false;
    }

    /*void GraphicTexture::FreeTexture()
    {
        if(IsAvalible())
        {
            glDeleteTextures(1,&GLPoint);
            GLPoint=0;
        }
    }*/

    void GraphicTexture::DrawTexture(const float *TxP,const float *LoP, int LSize,const float* CoP,int CSize,const unsigned int* Order,unsigned int All) const
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_TEXTURE_2D);
        glVertexPointer(LSize,GL_FLOAT,0,LoP);
        glColorPointer(CSize,GL_FLOAT,0,CoP);
        glTexCoordPointer(2,GL_FLOAT,0,TxP);
        glBindTexture(GL_TEXTURE_2D,GLPoint);
        glDrawElements(GL_TRIANGLES,All, GL_UNSIGNED_INT,Order);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
    }
    void GraphicTexture::DrawTexture(const float *TxP,const float *LoP, int LSize,const unsigned int* Order,unsigned int All) const
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_TEXTURE_2D);
        glVertexPointer(LSize,GL_FLOAT,0,LoP);
        glTexCoordPointer(2,GL_FLOAT,0,TxP);
        glBindTexture(GL_TEXTURE_2D,GLPoint);
        glDrawElements(GL_TRIANGLES,All,GL_UNSIGNED_INT,Order);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
    }

    void GraphicTexture::DrawTexture(const float* TxP,const float* LoP,int LSize,const float* CoP,int CSize,const float* NoP,const unsigned int* Order,unsigned int All) const
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnable(GL_TEXTURE_2D);
        glVertexPointer(LSize,GL_FLOAT,0,LoP);
        glColorPointer(CSize,GL_FLOAT,0,CoP);
        glTexCoordPointer(2,GL_FLOAT,0,TxP);
        glNormalPointer(GL_FLOAT,0,NoP);
        glBindTexture(GL_TEXTURE_2D,GLPoint);
        glDrawElements(GL_TRIANGLES,All, GL_UNSIGNED_INT,Order);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisable(GL_TEXTURE_2D);
    }

    GraphicTexture::GraphicTexture(const GraphicPreTexture& SS)
    {
        bool Change=false;
        TextureW=(SS.Sur)->w;
        TextureH=(SS.Sur)->h;
        MemoryW=TextureW,MemoryH=TextureH;
        RotateWH=static_cast<float>(TextureW)/TextureH;
        if((TextureW)&(TextureW-1))   {Change=true;MemoryW=Find2Power(TextureW);}
        if((TextureH)&(TextureH-1))   {Change=true;MemoryH=Find2Power(TextureH);}
        glGenTextures(1,&GLPoint);
        Texture2DEnvir TE(GLPoint);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        switch(SS.Sur->format->format)
        {
        case SDL_PIXELFORMAT_ABGR8888:
        {
            if(Change)
            {
                GraphicPreTexture GTD(SDL_CreateRGBSurface(0,MemoryW,MemoryH,32,0x000000ff,0x0000ff00,0x00ff0000, 0xff000000));
                SDL_BlitSurface(SS.Sur,NULL,GTD.Sur,NULL);
                glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,MemoryW,MemoryH,0,GL_RGBA,GL_UNSIGNED_BYTE,(GTD.Sur)->pixels);
            }else{
                glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,MemoryW,MemoryH,0,GL_RGBA,GL_UNSIGNED_BYTE,(SS.Sur)->pixels);
            }
        }
        break;
        case SDL_PIXELFORMAT_RGB24:
        {
            if(Change)
            {
                GraphicPreTexture GTD(SDL_CreateRGBSurface(0,MemoryW,MemoryH,24,0x000000ff,0x0000ff00,0x00ff0000, 0x00000000));
                SDL_BlitSurface(SS.Sur,NULL,GTD.Sur,NULL);
                glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,MemoryW,MemoryH,0,GL_RGB,GL_UNSIGNED_BYTE,(GTD.Sur)->pixels);
            }else
                glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,MemoryW,MemoryH,0,GL_RGB,GL_UNSIGNED_BYTE,(SS.Sur)->pixels);
        }
        break;
        case SDL_PIXELFORMAT_BGR888:
        {
            if(Change)
            {
                GraphicPreTexture GTD(SDL_CreateRGBSurface(0,MemoryW,MemoryH,24,0x000000ff,0x0000ff00,0x00ff0000, 0xff000000));
                SDL_BlitSurface(SS.Sur,NULL,GTD.Sur,NULL);
                glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,MemoryW,MemoryH,0,GL_RGB,GL_UNSIGNED_BYTE,(GTD.Sur)->pixels);
            }else
                glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,MemoryW,MemoryH,0,GL_RGB,GL_UNSIGNED_BYTE,(SS.Sur)->pixels);
        }
        break;
        case SDL_PIXELFORMAT_ARGB8888:
        {
            if(Change)
            {
                GraphicPreTexture GTD(SDL_CreateRGBSurface(0,MemoryW,MemoryH,32,0x000000ff,0x0000ff00,0x00ff0000, 0xff000000));
                SDL_BlitSurface(SS.Sur,NULL,GTD.Sur,NULL);
                glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,MemoryW,MemoryH,0,GL_RGBA,GL_UNSIGNED_BYTE,(GTD.Sur)->pixels);
            }else
                glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,MemoryW,MemoryH,0,GL_BGRA,GL_UNSIGNED_BYTE,(SS.Sur)->pixels);
        }
        break;
        default:
        {
            GraphicPreTexture GTD(SDL_CreateRGBSurface(0,MemoryW,MemoryH,32,0x000000ff,0x0000ff00,0x00ff0000, 0xff000000));
            SDL_BlitSurface(SS.Sur,NULL,GTD.Sur,NULL);
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,MemoryW,MemoryH,0,GL_RGBA,GL_UNSIGNED_BYTE,(GTD.Sur)->pixels);
        }
        break;
        }
    }
    GraphicTexture::~GraphicTexture(){glDeleteTextures(1,&GLPoint);}
    /*bool GraphicTexture::CreateTexture(SDL_Surface * Picture)
    {
        if(Picture)
        {
            glGenTextures(1,&P);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,P);
            bool Change=false;
            W=Picture->w;
            H=Picture->h;
            MW=W,MH=H;
            if((W)&(W-1))
            {
                Change=true;
                MW=Find2Power(W);
            }
            if((H)&(H-1))
            {
                Change=true;
                MH=Find2Power(H);
            }
            switch(Picture->format->format)
            {
            case SDL_PIXELFORMAT_ABGR8888:
                {
                    if(Change)
                    {
                        SDL_Surface* Picturenew=SDL_CreateRGBSurface(0,MW,MH,32,0x000000ff,0x0000ff00,0x00ff0000, 0xff000000);
                        SDL_BlitSurface(Picture,NULL,Picturenew,NULL);
                        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,MW,MH,0,GL_RGBA,GL_UNSIGNED_BYTE,Picturenew->pixels);
                        SDL_FreeSurface(Picturenew);
                    }else{
                        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,MW,MH,0,GL_RGBA,GL_UNSIGNED_BYTE,Picture->pixels);
                    }
                }
                break;
            case SDL_PIXELFORMAT_RGB24:
                {
                    if(Change)
                    {
                        SDL_Surface* Picturenew=SDL_CreateRGBSurface(0,MW,MH,24,0x000000ff,0x0000ff00,0x00ff0000, 0x00000000);
                        SDL_BlitSurface(Picture,NULL,Picturenew,NULL);
                        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,MW,MH,0,GL_RGB,GL_UNSIGNED_BYTE,Picturenew->pixels);
                        SDL_FreeSurface(Picturenew);
                    }else{
                        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,MW,MemoryH,0,GL_RGB,GL_UNSIGNED_BYTE,Picture->pixels);
                    }
                }
                break;
            case SDL_PIXELFORMAT_BGR888:
                {
                    if(Change)
                    {
                        SDL_Surface* Picturenew=SDL_CreateRGBSurface(0,MW,MemoryH,24,0x000000ff,0x0000ff00,0x00ff0000, 0xff000000);
                        SDL_BlitSurface(Picture,NULL,Picturenew,NULL);
                        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,MW,MemoryH,0,GL_RGB,GL_UNSIGNED_BYTE,Picturenew->pixels);
                        SDL_FreeSurface(Picturenew);
                    }else{
                        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,MW,MemoryH,0,GL_RGB,GL_UNSIGNED_BYTE,Picture->pixels);
                    }
                }
                break;
            case SDL_PIXELFORMAT_ARGB8888:
                {
                    if(Change)
                    {
                        SDL_Surface* Picturenew=SDL_CreateRGBSurface(0,MW,MemoryH,32,0x000000ff,0x0000ff00,0x00ff0000, 0xff000000);
                        SDL_BlitSurface(Picture,NULL,Picturenew,NULL);
                        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,MW,MemoryH,0,GL_RGBA,GL_UNSIGNED_BYTE,Picturenew->pixels);
                        SDL_FreeSurface(Picturenew);
                    }else{
                        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,MW,MemoryH,0,GL_BGRA,GL_UNSIGNED_BYTE,Picture->pixels);
                    }
                }
                break;
            default:
                {
                    SDL_Surface* Picturenew=SDL_CreateRGBSurface(0,MW,MemoryH,32,0x000000ff,0x0000ff00,0x00ff0000, 0xff000000);
                    SDL_BlitSurface(Picture,NULL,Picturenew,NULL);
                    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,MW,MemoryH,0,GL_RGBA,GL_UNSIGNED_BYTE,Picturenew->pixels);
                    SDL_FreeSurface(Picturenew);
                }
                break;
            }
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glDisable(GL_TEXTURE_2D);
            return true;
        }else{
            P=0;
            W=0;
            H=0;
            MW=0;
            MemoryH=0;
            return false;
        }
    }*/

    unsigned int GraphicTexture::Find2Power(unsigned int n)
    {
        n--;
        n |= n>>1;
        n |= n>>2;
        n |= n>>4;
        n |= n>>8;
        n |= n>>16;
        n++;
        return n;
    }
}
