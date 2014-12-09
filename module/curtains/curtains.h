#ifndef CURTAINS_H_INCLUDED
#define CURTAINS_H_INCLUDED
#include "../../plugin.h"
#include "../../graphic/figureset.h"
namespace PO
{
    /*class CurtainsSystemPrePlu;
    class CurtainsSystemPosPlu;
    class CurtainsSystem;

    class Curtains
    {
    public:
        /*typedef Adapter<CurtainsSystem,Render> CurAdapter;
        typedef Info<VertexInfo> CurtainsData;
        virtual bool IsFinish()=0;
        virtual void CurtainsInit(CurAdapter)=0;
        virtual void FadeOut(CurtainsData&,CurAdapter L,long Interval)=0;
        virtual void FadeIn(CurtansData&)=0;
        virtual void ClothChange(CurtainsData&,CurAdapter L,long Interval)=0;
    };

    struct CurtainsSystemState
    {
        bool IsTurning=false;
        bool CoverState=false;
        float Alpha=0.0;
    };

    class CurtainsSystem
    {
        bool Change;
        friend class CurtainsSystemPrePlu;
        friend class CurtainsSystemPosPlu;
        std::shared_ptr<Curtains> PosCurtains;
        std::shared_ptr<Curtains> PreCurtains;
        CurtainsSystemState PosState;
        CurtainsSystemState PreState;
    public:
        typedef CurtainsSystemPrePlu PrePlugin;
        typedef CurtainsSystemPosPlu PosPlugin;
    };

    class CurtainsSystemPrePlu:public Plugin<Render,CurtainsSystem>
    {
        typename Curtains::CurtainsData Screen;
    public:
        CurtainsSystemPrePlu(PluAdapter L){}
    };

    class CurtainsSystemPosPlu:public Plugin<Render,CurtainsSystem>
    {
        typename Curtains::CurtainsData Screen;
    public:
        CurtainsSystemPosPlu(PluAdapter){}
        void Layer2D(PluAdapter);
        void Layer3D(PluAdapter);
    };*/
}

#endif // COVERCLOTH_H_INCLUDED
