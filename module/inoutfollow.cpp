#include "inputfollow.h"
using namespace std;
using namespace PO;
void InputFollowPlu::Respond(const EventBox& EB,PluAdapter L)
{
    bool Mouse=false;
    for(auto& Li:EB.GetMouseBox())
    {
        GetMouseLocation(L,MouseLocation,Li);
        Mouse=true;
    }
    if(Mouse)
    {
        for(int i=0;i<2;++i)
            NT[i].Connect(L,MouseLocation(i));
    }
}
void InputFollowPlu::Layer2D(PluAdapter L)
{
    float SX, SY;
    InputFollow& IF(L);
    GetScreen2D(L,SX,SY);
    size_t One=NT[0].size();
    size_t Two=NT[1].size();
    size_t Max=One>Two?One:Two;
    Vertex<2> NS=MouseLocation;
    NS(1)-=IF.S*0.5;
    if(NS(0)+Max*IF.S*0.5>SX)
        NS(0)-=Max*IF.S*0.5;
    if(NS(1)+IF.S>=SY)
        NS(1)-=IF.S;
    else if(NS(1)-IF.S<=-SY)
        NS(1)+=IF.S;
    for(int i=0;i<2;++i)
        NT[i].DrawDynamic(L,DynTool::Nor(IF.S,NS(0),NS(1)+i*IF.S,IF.R,IF.G,IF.B,IF.A));
}
