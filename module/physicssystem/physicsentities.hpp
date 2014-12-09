#ifndef PHYSICSENTITIES_HPP_INCLUDED
#define PHYSICSENTITIES_HPP_INCLUDED
#include "../../glentity.hpp"
#include "physicseffect.hpp"

struct PhysicsProprty2D
{
    Vertex<2> Location;
    Normal<2> LineSpeed;
    float Angel;
    float RotationalSpeed;
    float Mess=1.0;
    Vertex<2> MessCenter;
    PhysicsProprty2D:Location(0.0,0.0),LineSpeed(0.0,0.0),Abgel(0),RotationalSpeed(0),Mess(1.0),MessCenter(0.0,0.0){}
};

struct ForceEffect2D
{
    Normal<2> LineAccelerated;
    Normal<2> LineImpluse;

    float RotationalImpluse;
    float RotationalAccelerated;

    void Reset()
    {
        LineAccelerated.Set(0,0);
        LineImpluse.Set(0,0);
        RotateAccelerated=0.0;
        Change=false;
    }
};

struct CrashFrame2D
{

};


/*template<unsigned int I>
struct PhysicsEntitie
{
    Vertex<I> Location;
    Normal<I> Speed;

    Normal<I> AcceleratedSpeed;
    Normal<I>
};

struct PhysicsEntitie*/

#endif // PHYSICSENTITIES_HPP_INCLUDED
