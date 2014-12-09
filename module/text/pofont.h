#ifndef FONTMANAGER_H
#define FONTMANAGER_H
#include <map>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../../graphic/figureset.h"
#include "../../language.h"
#include "../pomodule.h"
#include "../../render.h"

#include <iostream>
using namespace std;

namespace PO
{
    class FontData
    {
        TTF_Font* Data;
        operator TTF_Font* () {return Data;}
        friend class FontManager;
    public:
        FontData(TTF_Font* TTF) throw ( ResourceUnavailable ) :Data(TTF){ if(TTF==nullptr) throw ResourceUnavailable("Font");}
        ~FontData() {TTF_CloseFont(Data);}
    };

    typedef std::shared_ptr<FontData> Font;

    class SingleCharStruct
    {
        std::shared_ptr<GraphicTexture> Data;
        int Width;
        wchar_t Char;
        friend class FontManager;
    public:
        operator bool() const {return static_cast<bool>(Data);}
        wchar_t GetChar() const {return Char;}
        int GetWidth() const {return Width;}
        float GetRateW()const{return Data->GetRateW();}
        float GetRateH()const{return Data->GetRateH();}
        SingleCharStruct(const std::shared_ptr<GraphicPreTexture>& L,int W,wchar_t C):Data(new GraphicTexture(*L)),Width(W),Char(C){}
        SingleCharStruct(const GraphicPreTexture& L,int W,wchar_t C):Data(new GraphicTexture(L)),Width(W),Char(C){}
        void Swap(std::shared_ptr<GraphicTexture>&& L,int W,wchar_t C){Data.swap(L);Width=W;Char=C;}
        void Swap(std::shared_ptr<GraphicTexture>& L,int W,wchar_t C){Data.swap(L);Width=W;Char=C;}
        void ForceDelete(){Data.reset();}
        unsigned int GetIndex() const {return Data->GetPoint();}
    };

    template<template<typename StoreType,typename ...Other> class Tank>
        class DynamicTextData:public Tank<std::shared_ptr<SingleCharStruct> >
        {
            unsigned int WidthOfString;
            friend class FontManager;
        public:
            unsigned int GetWidth() const { return WidthOfString;  }
            DynamicTextData():WidthOfString(0){}
            DynamicTextData(const DynamicTextData& )=delete;
            void clear(){Tank<std::shared_ptr<SingleCharStruct>>::clear();WidthOfString=0;}
        };

    class StaticTextData:public TextureData
    {
        int Width=0;
        Font UsedFont;
        friend class FontManager;
        StaticTextData(){}
    public:
        int GetWidth(){return Width;}
        StaticTextData(const std::shared_ptr<GraphicPreTexture>& Tem,int W,Font& F):TextureData(Tem),Width(W),UsedFont(F){}
        StaticTextData(const GraphicPreTexture& Tem,int W,const Font& F):TextureData(Tem),Width(W),UsedFont(F){}
        void Swap(std::shared_ptr<GraphicTexture>&& L, int W){TextureData::Swap(L),Width=W;}
        void Swap(std::shared_ptr<GraphicTexture>& L, int W){TextureData::Swap(L),Width=W;}
    };

    class FontIninter
    {
        static unsigned int InitCount;
    public:
        FontIninter()
        {
            if(InitCount==0)
                TTF_Init();
            ++InitCount;
        }
        ~FontIninter()
        {
            --InitCount;
            if(InitCount==0)
                TTF_Quit();
        }
    };

    class FontManager:public FontIninter
    {
        std::map<wchar_t,std::shared_ptr<SingleCharStruct>> DynamicData;
        std::map<std::wstring,std::weak_ptr<StaticTextData>> StaticData;
        Font DefaultFont;
        static int InitCount;
        static SDL_Color Col;
    public:
        Font CreateFont(const std::string& Ade,int Size)throw ( FontUnavailable );
        void CreateDefaultFont( const std::string& Ade,int Size )throw ( FontUnavailable );

        template<template<typename Type,typename ...O>class  Tank >
            void CreateDynamicText(const std::string& U,DynamicTextData<Tank>& T)throw(  TextUnavailable  ){ CreateDynamicText(DefaultTurnToUnicode(U),T);}
        template<template<typename Type,typename ...O>class  Tank >
            void CreateDynamicText(const std::wstring& U,DynamicTextData<Tank>& T)throw(  TextUnavailable  ){ T.clear();AddDynamicText(U,T); }

        template<template<typename Type,typename ...O>class  Tank >
            void AddDynamicText(const std::string& U,DynamicTextData<Tank>& T)throw(  TextUnavailable  ){ CreateDynamicText(DefaultTurnToUnicode(U),T);}
        template<template<typename Type,typename ...O>class  Tank >
            void AddDynamicText(const std::wstring& U,DynamicTextData<Tank>& T)throw(  TextUnavailable  );

        std::shared_ptr<SingleCharStruct> CreateSingleDynamicText( wchar_t Data ) throw (TextUnavailable );

        std::shared_ptr<StaticTextData> CreateStaticText(const std::string& L)throw(  TextUnavailable,ResourceUnavailable  ){return CreateStaticText(L,DefaultFont);}
        std::shared_ptr<StaticTextData> CreateStaticText(const std::wstring& L)throw(  TextUnavailable,ResourceUnavailable  ){return CreateStaticText(L,DefaultFont);}
        std::shared_ptr<StaticTextData> CreateStaticText(const std::string& L,const Font& FI)throw(  TextUnavailable,ResourceUnavailable  ){return CreateStaticText(DefaultTurnToUnicode(L),FI);}
        std::shared_ptr<StaticTextData> CreateStaticText(const std::wstring& L,const Font& FI)throw(  TextUnavailable ,ResourceUnavailable );
        FontManager();
        FontManager(const std::string& L,int Size)throw ( FontUnavailable );
        ~FontManager();
        void PreWindowChange();
        void PosWindowChange(const GraphicInfo&,const GraphicInfo&);
    };


    template<template<typename Ts,typename ...TP> class S>
        void FontManager::AddDynamicText(const std::wstring& U, DynamicTextData<S>& T) throw(  TextUnavailable  )
        {
            for(auto& I:U)
            {
                std::shared_ptr<SingleCharStruct> Tem(CreateSingleDynamicText(I));
                T.WidthOfString+=Tem->Width;
                T.push_back(std::move(Tem));
            }
        }

    typedef FontManager FontModual;

    typedef std::shared_ptr<FontData> Font;
}
#endif // FONTMANAGER_H_INCLUDED
