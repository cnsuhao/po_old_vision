#include "posensitive.h"
namespace PO{
    bool MouseSensitive::GetLocation(unsigned int ID,float& X,float& Y)
    {
        auto Tp=AllMouse.find(ID);
        if(Tp==AllMouse.end())
            return false;
        X=Tp->second->CX;
        Y=Tp->second->CY;
        return true;
    }

    bool MouseSensitive::Respond(float MX,float MY,std::set<unsigned int>& TT)
    {
        bool Find=false;
        for(auto& I:AllMouse)
        {
            if((I.second)->RunPacket(MX-(I.second)->CX,MY-(I.second)->CY))
            {
                Find=true;
                TT.insert(I.first);
            }
        }
        return Find;
    }

    bool MouseSensitive::Change(unsigned int ID,float X,float Y)
    {
        auto Go=AllMouse.find(ID);
        if(Go==AllMouse.end()) return false;
        Go->second->CX=X;
        Go->second->CY=Y;
        return true;
    }

    bool MouseSensitive::Clear(unsigned int ID)
    {
        auto Go=AllMouse.find(ID);
        if(Go==AllMouse.end()) return false;
        AllMouse.erase(Go);
        return true;
    }
    bool MouseSensitive::SingleRespond(float MX,float MY,unsigned int K)
    {
        auto I=AllMouse.find(K);
        if(I==AllMouse.end())return false;
        if((I->second)->RunPacket(MX-(I->second)->CX,MY-(I->second)->CY)) return true;
        return false;
    }
}
