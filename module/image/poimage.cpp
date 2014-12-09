#include "poimage.h"
namespace PO
{
    unsigned int ImageManager::InitCount=0;

    void ImageManager::ClearRemain()
    {
        for(auto Po=ImageDataMap.begin();Po!=ImageDataMap.end();++Po)
        {
            if(Po->second.expired())
                ImageDataMap.erase(Po++);
            else Po++;
        }
    }

    ImageManager::~ImageManager()
    {
        for(auto& Po:ImageDataMap)
            if(!Po.second.expired())
                (Po.second.lock())->ForceDelete();
    }

    ImageManager::ImageIndex ImageManager::CreateImage(const std::string& U) throw( PictureUnavailable )
    {
        auto Find=ImageDataMap.find(U);
        if(Find!=ImageDataMap.end()&&!Find->second.expired())
        {
            return Find->second.lock();
        }else{
            GraphicPreTexture GTD(IMG_Load(U.c_str()));
            std::shared_ptr<TextureData> Tem(new TextureData(GTD));
            std::weak_ptr<TextureData> CC(Tem);
            ImageDataMap[U]=CC;
            return Tem;
        }
    }

    void ImageManager::ContextDelete()
    {
        for(auto L=ImageDataMap.begin();L!=ImageDataMap.end();)
        {
            if(L->second.expired())
                ImageDataMap.erase(L++);
            else{
                std::shared_ptr<TextureData> Tm=L->second.lock();
                Tm->ForceDelete();
                ++L;
            }
        }
    }
    void ImageManager::ContextRebuild()
    {
        for(auto L=ImageDataMap.begin();L!=ImageDataMap.end();++L)
        {
            auto P=L->second.lock();
            GraphicPreTexture Tem(IMG_Load(L->first.c_str()));
            std::shared_ptr<GraphicTexture> GT(  new GraphicTexture(Tem));
            P->Swap(std::move(GT));
        }
    }
}
