#include "pofont.h"
namespace PO
{
    unsigned int FontIninter::InitCount=0;
    SDL_Color FontManager::Col{255,255,255};

    Font FontManager::CreateFont(const std::string& Ade,int Size) throw ( FontUnavailable )
    {
        Font F(new FontData(TTF_OpenFont(Ade.c_str(),Size)));
        return F;
    }

    void FontManager::CreateDefaultFont( const std::string& Ade,int Size ) throw ( FontUnavailable )
    {
        DefaultFont=CreateFont(Ade,Size);
        for(auto &L : DynamicData)
        {
            std::wstring Tem;
            Tem+=L.first;
            GraphicPreTexture GPT(  TTF_RenderUNICODE_Blended(*DefaultFont,(Uint16*)(&Tem[0]),FontManager::Col)    );
            std::shared_ptr<GraphicTexture> GT(new GraphicTexture(GPT));
            L.second->Swap(GT,CountWidth(L.first),L.first);
        }
    }

    FontManager::FontManager(){}

    FontManager::FontManager(const std::string& L,int Size) throw ( FontUnavailable )
    {
        if(!L.empty())
            CreateDefaultFont( L,Size );
    }

    void FontManager::PreWindowChange()
    {
        for(auto P=StaticData.begin();P!=StaticData.end();)
            if(P->second.expired())
            {
                StaticData.erase(P++);
            }else{
                (P->second.lock())->ForceDelete();
                ++P;
            }
        for(auto P=DynamicData.begin();P!=DynamicData.end();)
        {
            if(P->second.use_count()==1)
                DynamicData.erase(P++);
            else{
                P->second->ForceDelete();
                ++P;
            }
        }
    }

    void FontManager::PosWindowChange(const GraphicInfo&,const GraphicInfo&)
    {
        for(auto& P:StaticData)
        {
            auto Yi=P.second.lock();
            GraphicPreTexture TC(TTF_RenderUNICODE_Blended(*Yi->UsedFont,(Uint16*)(P.first.c_str()),Col));
            std::shared_ptr<GraphicTexture> KK(new GraphicTexture(TC));
            Yi->Swap(KK,CountWidth(P.first));
        }
        for(auto& P:DynamicData)
        {
            std::wstring L;
            L+=P.first;
            GraphicPreTexture TC(TTF_RenderUNICODE_Blended(*DefaultFont,(Uint16*)L.c_str(),Col));
            std::shared_ptr<GraphicTexture> KK(new GraphicTexture(TC));
            (P.second)->Swap(KK,CountWidth(L),P.first);
        }
    }

    FontManager::~FontManager()
    {
        for(auto& I:StaticData)
        {
            if(!I.second.expired())
            {
                auto O=I.second.lock();
                O->ForceDelete();
            }
        }
        for(auto& I:DynamicData)
        {
            (I.second)->ForceDelete();
        }
    };


    std::shared_ptr<StaticTextData> FontManager::CreateStaticText(const std::wstring& L,const Font& FI) throw(  TextUnavailable,ResourceUnavailable  )
    {
        auto Yi=StaticData.find(L);
        if(Yi!=StaticData.end()&&!Yi->second.expired())
            return Yi->second.lock();
        else if(!L.empty())
        {
            GraphicPreTexture GPT(TTF_RenderUNICODE_Blended(*FI,(Uint16*)(L.c_str()),FontManager::Col));
            std::shared_ptr<StaticTextData> Tem(new StaticTextData(GPT,CountWidth(L),FI));
            std::weak_ptr<StaticTextData> KK(Tem);
            StaticData[L]=KK;
            return Tem;
        }else{
            std::shared_ptr<StaticTextData> Tem(new StaticTextData);
            return Tem;
        }
    }

    std::shared_ptr<SingleCharStruct> FontManager::CreateSingleDynamicText( wchar_t Data ) throw (TextUnavailable )
    {
        auto Find=DynamicData.find(Data);
        if(Find!=DynamicData.end())
        {
            auto Tem=Find->second;
            return Tem;
        }else{
            wchar_t TemCC[2];
            TemCC[0]=Data;
            TemCC[1]=0;
            GraphicPreTexture GPT(TTF_RenderUNICODE_Blended(*DefaultFont,(Uint16*)(TemCC),FontManager::Col));
            std::shared_ptr<SingleCharStruct> Tem(new SingleCharStruct(GPT,CountWidth(Data),Data));
            DynamicData.insert(std::make_pair(Data,Tem));
            return Tem;
        }
    }
}
