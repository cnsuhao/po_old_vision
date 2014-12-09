#ifndef GRAPHICENVIRONMENTALPAIR_H_INCLUDED
#define GRAPHICENVIRONMENTALPAIR_H_INCLUDED
namespace PO
{
    struct Texture2DEnvir
    {
        Texture2DEnvir(unsigned int P)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,P);
        }
        ~Texture2DEnvir()
        {
            glDisable(GL_TEXTURE_2D);
        }
    };
}


#endif // GRAPHICENVIRONMENTALPAIR_HPP_INCLUDED
