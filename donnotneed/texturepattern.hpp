#ifndef TEXTUREPATTERN_HPP_INCLUDED
#define TEXTUREPATTERN_HPP_INCLUDED
#include <vector>
#include <functional>
#include "gltexture.hpp"
#include "glentity.hpp"
namespace PO
{

    /*struct PicturePattern:public TexturePattern
    {
    };

    struct TextStaticPattern:public TexturePattern
    {
    };*/



    /*void Translate(TexturePattern& L,const Point<2>& P)
    {
        if(L.LPS==2)
        {
            L.Loc.clear();
            L.LS=2;
            for(int i=0;i<L.LocPat.size();i+=2)
            {
                L.Loc.push_back(L.LocPat[i]+P(0));
                L.Loc.push_back(L.LocPat[i+1]+P(1));
            }
        }else if(L.LPS==3)
        {
            L.Loc.clear();
            L.LS=2;
            for(int i=0;i<L.LocPat.size();i+=3)
            {
                L.Loc.push_back(L.LocPat[i]+P(0));
                L.Loc.push_back(L.LocPat[i+1]+P(1));
            }
        }
    }*/

    /*auto SquE2D_S=std::bind(
                                [](const GLTexture& GL,TexturePattern& PD,float Size)
                                {
                                    PD.Clear();
                                    if(GL.IsAvalible())
                                    {
                                        PD.LPS=2;
                                        PD.CS=3;
                                        PD.Dra={0,1,2,2,0,3};
                                        PD.Tex={   0.0,GL.GetRateH() ,
                                                    GL.GetRateW(),GL.GetRateH(),
                                                    GL.GetRateW(),0.0,
                                                    0.0,0.0
                                                    };
                                        PD.Col={  1.0,1.0,1.0,
                                                    1.0,1.0,1.0,
                                                    1.0,1.0,1.0,
                                                    1.0,1.0,1.0};
                                        PD.LocPat={    0.0,0.0,
                                                                GL.GetRateWH()*Size,0.0,
                                                                GL.GetRateWH()*Size,Size,
                                                                0.0,Size};
                                        PD.LS=PD.LPS;
                                        PD.Loc=PD.LocPat;
                                    }
                                },
                                std::placeholders::_1,std::placeholders::_2,std::placeholders::_3
                           );


    auto SquE2D_SC4=std::bind(
                                [](const GLTexture& GL,TexturePattern& PD,float Size,const Color<4>& C)
                                {
                                    PD.Clear();
                                    if(GL.IsAvalible())
                                    {
                                        PD.LPS=2;
                                        PD.CS=4;
                                        PD.Dra={0,1,2,2,0,3};
                                        PD.Tex={   0.0,GL.GetRateH() ,
                                                    GL.GetRateW(),GL.GetRateH(),
                                                    GL.GetRateW(),0.0,
                                                    0.0,0.0
                                                    };
                                        PD.Col={  C(0),C(1),C(2),C(3),
                                                    C(0),C(1),C(2),C(3),
                                                    C(0),C(1),C(2),C(3),
                                                    C(0),C(1),C(2),C(3)};
                                        PD.LocPat={    0.0,0.0,
                                                                GL.GetRateWH()*Size,0.0,
                                                                GL.GetRateWH()*Size,Size,
                                                                0.0,Size};
                                        PD.LS=PD.LPS;
                                        PD.Loc=PD.LocPat;
                                    }
                                },
                                std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4
                           );*/

}

#endif // TEXTUREPATTERN_HPP_INCLUDED
