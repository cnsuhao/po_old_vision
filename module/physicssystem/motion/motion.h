#ifndef MOTION_H_INCLUDED
#define MOTION_H_INCLUDED
#include "../../../elementfigure.h"
namespace PO{
    template<unsigned int Size>
        struct LineMotion
        {
            Vertex<Size> Location;
            Normal<Size> LineSpeed;
            Normal<Size> LineAcceleration;
            operator Vertex<Size>& () {return Location;}
            void LineUpDate(float L)
            {
                LineSpeed+=L*LineAcceleration;
                Location+=L*LineSpeed;
                LineAcceleration.Reset();
            }
            void AddLineAcceleration( const Normal<Size>& N)
            {
                LineAcceleration+=N;
            }
            void AddLineSpeed( const Normal<Size>& N)
            {
                LineSpeed+=N;
            }
        };

        struct RotateMotion_2D
        {
            float Angle=0;
            float RotateSpeed=0;
            float RotateAcceleration=0;
            operator float (){return Angle;}
            void RotateUpDate( float S)
            {
                RotateSpeed+=RotateAcceleration*S;
                RotateAcceleration=0.0;
                Angle+=RotateSpeed*S;
            }
            void AddRotateAcceleration( float N)
            {
                RotateAcceleration+=N;
            }
            void AddRotateSpeed( float N)
            {
                RotateSpeed+=N;
            }
        };

        struct RotateMotion_3D
        {
            Normal<3> Angle;
            Normal<3> RotateSpeed;
            Normal<3> RotateAcceleration;
            operator Normal<3>& (){return Angle;}
            void RotateUpDate( float S )
            {
                RotateSpeed+=S*RotateAcceleration;
                RotateAcceleration.Reset();
                Angle+=S*RotateSpeed;
            }
            void AddRotateAcceleration( const Normal<3>& N)
            {
                RotateAcceleration+=N;
            }
            void AddRotateSpeed( const Normal<3>& N)
            {
                RotateSpeed+=N;
            }
        };

    class Motion2D:public LineMotion<2>,public RotateMotion_2D
    {
    public:
        void UpData(float A)
        {
            LineUpDate(A);
            RotateUpDate(A);
        }
    };

    class Motion3D:public LineMotion<3>,public RotateMotion_3D
    {
    public:
        void UpData(float A)
        {
            LineUpDate(A);
            RotateUpDate(A);
        }
    };
}


#endif // MOTION_H_INCLUDED
