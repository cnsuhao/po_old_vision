#ifndef IDALLOCATE_H_INCLUDED
#define IDALLOCATE_H_INCLUDED
#include <deque>
template<typename T,int Start=0>
class IDAllocator
{
public:
    typedef int IDType;
private:
    static IDType StartID;
    static std::vector<IDType> UsedID;
    static IDType Allocate()
    {
        if(!UsedID.empty())
        {
            IDType Tem=*UsedID.rbegin();
            UsedID.pop_back();
            return Tem;
        }
        else    return ++StartID;
    }
    static void Delete( IDType I)
    {
        if(I==StartID-1)StartID--;
        else UsedID.push_back(I);
    }
public:

    bool IsEmptyID(IDType I){return I==Start;}
    class ID
    {
        IDType ThisID;
    public:
        ID():ThisID(IDAllocator::Allocate()){}
        ~ID(){IDAllocator::Delete(ThisID);}
        bool SameID(const ID& I) const {return I.ThisID==ThisID;}
        bool SameID(const IDType& I) const {return I==ThisID;}
        IDType GetID() const {return ThisID;}
    };
    static int GetMaxID(){return StartID;}

};
template<typename T,int Start> int IDAllocator<T,Start>::StartID=Start+1;
template<typename T,int Start> std::vector<int> IDAllocator<T,Start>::UsedID;


#endif // IDALLOCATE_H_INCLUDED
