#ifndef PHYSICSEFFECT_HPP_INCLUDED
#define PHYSICSEFFECT_HPP_INCLUDED
namespace PO
{
    struct Force:public Normal<2>
    {
        Vertex<2> ForcePoint;
    };

    struct Impulse:public Normal<2>
    {
        Vertex<2> ImpulsePoint;
    };

    struct Constraint:public Normal<2>
    {
        Vertex<2> Point;
    };
}


#endif // PHYSICSEFFECT_HPP_INCLUDED
