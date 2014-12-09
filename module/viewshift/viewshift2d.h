#ifndef VIEWSHIFT2D_H_INCLUDED
#define VIEWSHIFT2D_H_INCLUDED
#include "../../POContext.h"
#include "../../elementfigure.h"
class ViewShift2D
{
    float AcceLine;
    PO::Vertex<float,2> Speed;
    PO::Vertex<float,2> Location;
    PO::Vertex<float,2> Target;
    float SqrDeviation;
    float MaxValueSqr;
    unsigned int State;
public:
    ViewShift2D(const Vertex<float,2>& Lo={0.0,0.0}, float ACL=1.0, float MaxD=1.0,float Sqr=0.00001 );
    void PreRender(PO::Render&,PO::POContext&);
    void GetRealLocation(float& X,float& Y){X+=LocationX;Y+=LocationY;}

};

#endif // VIEWSHIFT2D_H_INCLUDED
