#ifndef VIEWSHIFT2D_H_INCLUDED
#define VIEWSHIFT2D_H_INCLUDED
#include "../../POContext.h"
#include "../../elementfigure.h"
class ViewShift2D
{
    float AcceLine;
    Vertex<2> Speed;
    Vertex<2> Location;
    Vertex<2> Target;
    float SqrDeviation;
    float MaxValueSqr;
    unsigned int State;
public:
    //ViewShift2D(const Vertex<2>& Lo={0.0,0.0}, float ACL=1.0, float MaxD=1.0,float Sqr=0.00001 );
    void PreRender(PO::Render&,PO::POContext&);
    void GetRealLocation(float& X,float& Y){X+=Location(1);Y+=Location(2);}

};

#endif // VIEWSHIFT2D_H_INCLUDED
