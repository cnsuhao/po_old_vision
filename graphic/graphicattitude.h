#ifndef GRAPHICATTITUDE_H_INCLUDED
#define GRAPHICATTITUDE_H_INCLUDED
namespace PO
{
    struct VertexArrayAttitude
    {
        bool Ver_Arr_En=false;
        bool Nor_Arr_En=false;
        bool Ble_En=false;
        bool Col_Arr_En=false;
        bool Tex_Coo_Arr_En=false;
        void OpenNorArr(const std::vector<float>& L)
        {
            if(!Nor_Arr_En)
            {
                glEnableClientState(GL_NORMAL_ARRAY);
                Nor_Arr_En=true;
            }
            glNormalPointer(GL_FLOAT,0,&L[0]);
        }
        void CloseNorArr()
        {
            if(Nor_Arr_En)
            {
                glDisableClientState(GL_NORMAL_ARRAY);
                Nor_Arr_En=false;
            }
        }
        void OpenVerArr(const std::vector<float>& L,size_t S)
        {
            if(!Ver_Arr_En)
            {
                glEnableClientState(GL_VERTEX_ARRAY);
                Ver_Arr_En=true;
            }
            glVertexPointer(S,GL_FLOAT,0,&L[0]);
        }
        void CloseVerArr()
        {
            if(Ver_Arr_En)
            {
                glDisableClientState(GL_VERTEX_ARRAY);
                Ver_Arr_En=false;
            }
        }
        void OpenColArr(const std::vector<float>& L,size_t S)
        {
            if(!Col_Arr_En)
            {
                glEnableClientState(GL_COLOR_ARRAY);
                Col_Arr_En=true;
            }
            glColorPointer(S,GL_FLOAT,0,&L[0]);
        }
        void CloseColArr()
        {
            if(Col_Arr_En)
            {
                glDisableClientState(GL_COLOR_ARRAY);
                Col_Arr_En=false;
            }
        }
        void OpenTexCooArr(const std::vector<float>& L)
        {
            if(!Tex_Coo_Arr_En)
            {
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                Tex_Coo_Arr_En=true;
            }
            glTexCoordPointer(2,GL_FLOAT,0,&L[0]);
        }
        void CloseTexCooArr()
        {
            if(Tex_Coo_Arr_En)
            {
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                Tex_Coo_Arr_En=false;
            }
        }
        void CloseAll()
        {
            CloseColArr();
            CloseNorArr();
            CloseVerArr();
            CloseTexCooArr();
        }
    };
    struct GraphicAttitude:public VertexArrayAttitude
    {
        bool BeenLocked=false;
        bool Tex_2D_En=false;
        bool Ble_En=false;
        bool Blend_En=false;
        void OpenTex2D(unsigned int P)
        {
            if(!Tex_2D_En)
            {
                glEnable(GL_TEXTURE_2D);
                Tex_2D_En=true;
            }
            glBindTexture(GL_TEXTURE_2D,P);
        }
        void CloseTex2D()
        {
            if(Tex_2D_En)
            {
                glDisable(GL_TEXTURE_2D);
                Tex_2D_En=false;
            }
        }
        void CloseAll()
        {
            VertexArrayAttitude::CloseAll();
            CloseTex2D();
        }
    };
}

#endif // GRAPHICATTITUDE_H_INCLUDED
