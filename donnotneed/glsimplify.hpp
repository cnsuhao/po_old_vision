#ifndef DIY_GLSIMPLIFY_HPP
#define DIY_GLSIMPLIFY_HPP
#include <GL/gl.h>
#include "vector.hpp"
#include "squarematrix.hpp"
inline void glVertex(const float X,const float Y){glVertex2f(X,Y);}
inline void glVertex(const Vector<float,4>& Tem){glVertex3f(Tem(0),Tem(1),Tem(2));}
inline void glVertex(const Vector<float,3>& Tem){glVertex2f(Tem(0),Tem(1));}
inline void glNormal(const Vector<float,3>& Tem){glNormal3f(Tem(0),Tem(1),Tem(2));}

void AddMatrix(const Matrix<float,4,4>& P)
{
    static float M[16];
    for(int i=0;i<4;++i)
        for(int k=0;k<4;++k)
            M[i*4+k]=P(i,k);
    glMultMatrixf(M);
}
//void DrawQuad(float A,float B,float C,float D)
#endif
