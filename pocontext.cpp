#include "pocontext.h"
namespace PO
{
    void POContext::DeliverEvent( const std::shared_ptr<Event>& Tem )
    {
        unsigned int Index;
        if(Tem->GetWindowIndex(Index))
        {
            auto PO=RenderMap.find(Index);
            if(PO!=RenderMap.end())
                PO->second->GetEvent(Tem);
        }else{
            for( auto& L:RenderMap  )
                (L.second)->GetEvent(Tem);
        }
    }

    int POContext::ContextLoop()
    {
        while((!RenderMap.empty()))
        {
            std::shared_ptr<Event> Temp;
            while(Graphic::GetEvent(Temp))
            {
                if(Temp->IsExit())
                    return 0;
                DeliverEvent(Temp);
            }
            for(auto Po=RenderMap.begin(); Po!=RenderMap.end();)
            {
                if(Po->second->CheckExitSignal())
                    RenderMap.erase(Po++);
                else
                {
                    Po->second->RenderLoop(*this);
                    /*Po->second->PreRender();
                    Po->second->SetCurrent();
                    Po->second->DeliverEvent(*this);
                    Po->second->CleanScreen();
                    Po->second->SetParallel();
                    Po->second->Render2D(*this);
                    Po->second->SetPerspective();
                    Po->second->Render3D(*this);
                    Po->second->FinishLoop();
                    Po->second->PosRender();
                    Po->second->SwapScreen();*/
                    Po++;
                }
            }
            long Inter=Graphic::GetTick()-SpendTime;
            if(Inter<17)
                Graphic::Delay(17-Inter);
            SpendTime=Graphic::GetTick();
        }
        return 0;
    }
}
