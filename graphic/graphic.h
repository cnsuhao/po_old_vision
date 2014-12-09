#ifndef GRAPHIC_H_INCLUDE
#define GRAPHIC_H_INCLUDE
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include "event.h"
#include "../error/poerror.h"
#include "graphicenvironmentalpair.h"
#include "../language.h"
#include "graphicattitude.h"
#include <iostream>
using namespace std;
namespace PO
{
    class GraphicInfo
    {
        Uint32 Flag=SDL_WINDOW_OPENGL;
        int WindowW=640,WindowH=480;
        float WindowScale=640/480.0;
        std::string Title="PO Frame";
        float Screen2DY=1.0,ScreenScale=640/480.0;
        float Screen3DAngle=95,Screen3DNear=0.05,Screen3DFar=100.0;
        bool Active=true;
        friend class GraphicContext;
    public:
        float GetScreen2DX()const {return Screen2DY*ScreenScale;}
        float GetScreen2DY()const {return Screen2DY;}
        void SetWindow(int W,int H){WindowW=W;WindowH=H;WindowScale=static_cast<float>(WindowW)/WindowH;ScreenScale=WindowScale;}
        void Set3DView(float Angle,float SN,float SF) { Screen3DAngle=Angle;Screen3DNear=SN;Screen3DFar=SF;   }
        void SetTitle(const std::string& L){Title=L;}
        void SetFullScreen(){Flag|=SDL_WINDOW_FULLSCREEN;}
        void SetWindowScreen(){Flag&=~SDL_WINDOW_FULLSCREEN;}
        void Set2DView(float X,float Y){Screen2DY=Y;ScreenScale=X/Y;}
        int GetWindowW()    const   {return WindowW;}
        int GetWindowH()    const   {return WindowH;}
    };

    class GraphicContext:public GraphicInfo,public GraphicAttitude
    {
        SDL_Window* MainSurface=nullptr;
        SDL_GLContext GLContext;
        EventBox EB;
        unsigned int WindowIndex=0;
        long StartTime,SpendTime,IntervalTime;
        GraphicContext(const GraphicContext& GC)=delete;

    public:

        void ResetWindowSize(int X,int Y);
        long GetStartTime() const {return StartTime;}
        long GetSpendTime() const {return SpendTime;}
        long GetIntervalTime() const {return IntervalTime;}

        int GetWindowIndex() const {return WindowIndex;}
        /*float GetScreen2DX()const {return GraphicInfo::GetScreen2DX();}
        float GetScreen2DY()const {return GraphicInfo::GetScreen2DY();}
        int GetWindowW()    const   {return GraphicInfo::GetWindowW();}
        int GetWindowH()    const   {return GraphicInfo::GetWindowH();}
        void Set2DView(float X,float Y){GraphicInfo::Set2DView(X,Y);}*/

        void CleanScreen(){glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);}
        void SwapScreen(){SDL_GL_SwapWindow(MainSurface);}
        void SetCurrent();

        void SetPerspective();
        void SetParallel();
        void PointCalculate(int Xi,int Yi,float &R,float &P) const;

        GraphicContext(const GraphicInfo& GI) throw( CreatingGLContextFail,CreatingWindowFail   );
        ~GraphicContext();
    };

    inline std::shared_ptr<GraphicInfo> CreateGraphicInfo(){std::shared_ptr<GraphicInfo> Tem(new GraphicInfo());return Tem;}

    class GraphicPreTexture
    {
        SDL_Surface* Sur;
        friend class GraphicTexture;
        GraphicPreTexture(const GraphicPreTexture&)=delete;
        GraphicPreTexture& operator= (const GraphicPreTexture&)=delete;
    public:
        ~GraphicPreTexture(){SDL_FreeSurface(Sur);}
        GraphicPreTexture(SDL_Surface* SS) throw (ResourceUnavailable):Sur(SS) { if(SS==nullptr) throw ResourceUnavailable("Unable To Create Such Surface");  }
    };

    class GraphicTexture
    {
        unsigned int GLPoint;
        unsigned int TextureW,TextureH;
        unsigned int MemoryW,MemoryH;
        float RotateWH;
        //Well, as we all know, gl can not store any picture witch 2^n*2^n, so any picture loaded should be changes to2^n*2^n,
        //and changes to normal when displaying.
        //static bool CreateTexture(SDL_Surface*,unsigned int& P,unsigned int& W,unsigned int& H,unsigned int & MemoryW,unsigned int & MemoryH);
        //static bool CreateTexture(SDL_Surface* K,Texture& P){P.FreeTexture();return CreateTexture(K,P.GLPoint,P.TextureW,P.TextureH,P.MemoryW,P.MemoryH);}
        GraphicTexture(const GraphicTexture& K)=delete;
        static unsigned int Find2Power(unsigned int );
    public:
        void DrawTexture(const float* TxP,const float* LoP,int LSize,const unsigned int* Or,unsigned int M) const ;
        void DrawTexture(const float* TxP,const float* LoP,int LSize,const float* CoP,int CSize,const unsigned int* Or,unsigned int M) const ;
        void DrawTexture(const float* TxP,const float* LoP,int LSize,const float* CoP,int CSize,const float* NoP,const unsigned int* Or,unsigned int M) const ;
        unsigned int GetPoint() const {return GLPoint;}
        unsigned int GetTextureW() const {return TextureW;}
        unsigned int GetTextureH() const {return TextureH;}
        unsigned int GetMemoryW()const  {return MemoryW;}
        unsigned int GetMemoryH()const  {return MemoryH;}
        float GetRateWH() const {return RotateWH;}
        float GetRateW()const  {return static_cast<float>(TextureW)/MemoryW;}
        float GetRateH()const  {return static_cast<float>(TextureH)/MemoryH;}
        bool IsAvalible() const {return glIsTexture(GLPoint)==GL_TRUE;}
        GraphicTexture(const GraphicPreTexture& SS);
        //GraphicTexture(std::shared_ptr<GraphicTextureData> Tem):GraphicTexture(*Tem){}
        //GraphicTexture(std::shared_ptr<GraphicTextureData>&& Tem):GraphicTexture(*Tem){}
        //GraphicTexture(std::shared_ptr<GraphicTextureData>& Tem):GraphicTexture(*Tem){}
        ~GraphicTexture();
    };

    class Graphic
    {
    public:
        static void Init(Uint32 Flag=0){SDL_Init(SDL_INIT_EVENTS|SDL_INIT_TIMER|SDL_INIT_VIDEO|Flag|SDL_INIT_AUDIO);}
        static void Quit(){SDL_Quit();}
        static bool GetEvent(std::shared_ptr<Event>& I);
        static void Delay(long Time){SDL_Delay(Time);}
        static void SetPerspective(const GraphicContext& Win);
        static void SetParallel(const GraphicContext& Win);
        static long GetTick(){return SDL_GetTicks();}
        static std::string TellSurfaceFormat(GraphicTexture);
    };
}
#endif // Graphic_H
