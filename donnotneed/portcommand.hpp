#ifndef PORTCOMMAND_HPP_INCLUDED
#define PORTCOMMAND_HPP_INCLUDED
#include "..\handle.hpp"
namespace Port
{
    const unsigned int PORT_WAIT=2,PORT_SENT=1,PORT_LOOP=3,PORT_DEADLOOP=4,PORT_QUIT=5,PORT_JUMP=6,PORT_LABEL=7;
        struct PortCommand:public HandleSeed
    {
        virtual unsigned int Type()=0;
        virtual int Datai(){return 0;}
        virtual void* Datav(){return nullptr;}
        virtual unsigned char Datauc(){return 0;}
        virtual unsigned long Dataul(){return 0;}
        virtual unsigned int Dataui(){return 0;}
        virtual bool Datab(){return false;}
        virtual ~PortCommand(){}
    };

    struct WaitCommand:public PortCommand
    {
        unsigned long WaitTime;
        virtual unsigned int Type(){return PORT_WAIT;}
        virtual unsigned long Data(){return WaitTime;}
        WaitCommand(unsigned long WT):WaitTime(WT){}
    };

    struct SentCommand:public PortCommand
    {
        unsigned int Size;
        char* SentData;
        unsigned int Dataui(){return Size;}
        SentCommand(void* Data,unsigned int S)
        {
            if((Size=S)>0)
                try{
                    SentData=new char[S];
                }catch(std::bad_alloc)
                {
                    Size=0;
                    SentData=nullptr;
                    return ;
                }
        }

        virtual unsigned int Type(){return PORT_SENT;}
        virtual void* Data(){return &SentData;}
        ~SentCommand(){if(Size!=0) delete [] SentData;}
    };

    struct JumpCommand:public PortCommand
    {
        int JumpCount;
        unsigned long TargetTime;
        JumpCommand(int ID,unsigned long TT):JumpCount(ID),TargetTime(TT){}
        virtual unsigned int Type(){return PORT_JUMP;}
        unsigned long Dataul(){return TargetTime;}
        unsigned int Dataui(){return JumpCount;}
    };
}

#endif // PORTCOMMAND_HPP_INCLUDED
