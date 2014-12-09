#ifndef INPUTFOLLOW_H_INCLUDED
#define INPUTFOLLOW_H_INCLUDED
#include "text/potext.h"
#include "../plugin.h"
#include <iostream>
class InputFollow;
class InputFollowPlu:public PO::Plugin<PO::FontManager,InputFollow>
{
    PO::NumberText NT[2];
    Vertex<2> MouseLocation;
public:
    InputFollowPlu(PluAdapter L){}
    void Respond(const PO::EventBox&,PluAdapter);
    void Layer2D(PluAdapter L);
};
struct InputFollow
{
    float R,G,B,A,S;
    typedef InputFollowPlu PosPlugin;
    InputFollow(float Si=0.1,float Re=1.0,float Gr=1.0,float Bl=1.0,float Al=1.0):R(Re),G(Gr),B(Bl),A(Al),S(Si){}
    void NorPluCreate(PO::Render&,PO::POContext&,const std::type_info& I)
    {
        std::cout<<"Create Type: "<<I.name()<<std::endl;
    }
    void NorPluDelete(PO::Render&,PO::POContext&,const std::type_info& I)
    {
        std::cout<<"Delete Type: "<<I.name()<<std::endl;
    }
};


#endif // INPUTFOLLOW_H_INCLUDED
