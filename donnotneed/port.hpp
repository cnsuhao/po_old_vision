#ifndef PORT_HPP_INCLUDED
#define PORT_HPP_INCLUDED
#include "winserialport.h"
#include "portcommand.hpp"
#include <functional>
#include <thread>
#include <mutex>
#include "..\handle.hpp"
#include <vector>
#include <deque>
#include <map>
namespace Port
{
    class SerialContext
    {
        std::string Name;
        unsigned int Bit;
    public:
        SerialContext(const std::string& K,unsigned int B):Name(K),Bit(B){}
        //SerialContext()
        const std::string& GetSerialName()const {return Name;}
        unsigned int GetBit()const{return Bit;}

    };

    class Serial
    {
        WinSerialPort WSP;
        bool OutSignal=false;
        bool CommandReflesh=false;
        std::mutex CommandLock;
        std::vector<Handle<PortCommand>> CommandVector;
        std::deque<char> InputData;
        std::thread SerialThread;

        void Lock(){CommandLock.lock();}
        void UnLock(){CommandLock.unlock();}

        void ResetEnvironment();
        void GetInput();
        static void SerialFunction(Serial* S);
    public:
        bool CreatSerial(const SerialContext& C)
        {
            ResetEnvironment();
            WSP.CreatSerial(C.GetSerialName(),C.GetBit());
            if(WSP.IsOpen()){
                SerialThread=std::thread(SerialFunction,this);
                return true;
            }else return false;
        }

        void CloseSerial(){ResetEnvironment();}

        Serial(const Serial& )=delete;
        Serial(){}
        ~Serial(){ResetEnvironment();}
    };

    void Serial::GetInput()
    {
        unsigned int K=WSP.ReadBuffer();
        if(K>0)
        {
            DWORD Unknow;
            char* Tem=new char[K];
            WSP.ScanfInput(Tem,K,Unknow);
            for(int i=0;i<K;++i)
                InputData.push_back(Tem[i]);
            delete[](Tem);
        }
    }


    void Serial::ResetEnvironment()
    {
        Lock();
        CommandVector.clear();
        OutSignal=true;
        CommandReflesh=true;
        InputData.clear();
        UnLock();
        if(SerialThread.joinable())
            SerialThread.joinable();
        WSP.CloseSerial();
    }

    void Serial::SerialFunction(Serial* S)
    {
        int CommandStep=0;
        int MaxStep=0;
        std::map<int,int> JumpCount;
        while(1)
        {
            S->Lock();
            bool Out=S->OutSignal;
            if(Out)
            {
                S->UnLock();
                return ;
            }else{
                bool CommandChange=S->CommandReflesh;
                if(CommandChange)
                {
                    MaxStep=S->CommandVector.size();
                    CommandStep=0;
                    JumpCount.clear();
                    S->CommandReflesh=false;
                }
                if(CommandStep!=MaxStep)
                {
                    Handle<PortCommand> Tem=S->CommandVector[CommandStep];
                    unsigned int Type=Tem->Type();
                    switch(Type)
                    {
                    case PORT_SENT:
                        {
                            unsigned int Size=Tem->Dataui();
                            char *Ty=new char[Size];
                            for(int i=0;i<Size;++i)
                                Ty[i]=((char*)Tem->Datav())[i];
                            S->GetInput();
                            S->UnLock();
                            if(S->WSP.IsOpen())
                            {
                                DWORD USize;
                                S->WSP.Write(Ty,Size,USize);
                            }
                            delete [](Ty);
                            ++CommandStep;
                        }
                        break;
                    case PORT_WAIT:
                        {
                            unsigned long WaitTime=Tem->Dataul();
                            S->UnLock();
                            while(WaitTime>0)
                            {
                                if(WaitTime>10)
                                {
                                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                                    WaitTime-=10;
                                }else{
                                    std::this_thread::sleep_for(std::chrono::milliseconds(WaitTime));
                                    WaitTime=0;
                                }
                                S->Lock();
                                bool Quit=S->OutSignal||S->CommandReflesh;
                                if(!Quit)
                                    S->GetInput();
                                S->UnLock();
                                if(Quit)
                                    break;
                            }
                            ++CommandStep;
                        }
                        break;
                    case PORT_JUMP:
                        {
                            unsigned long Time=Tem->Dataul();
                            int Jump=Tem->Datai();
                            S->UnLock();
                            if(JumpCount[CommandStep]<Time)
                            {
                                ++JumpCount[CommandStep];
                                CommandStep+=Jump;
                            }else{
                                JumpCount[CommandStep]=0;
                            }
                        }
                        break;
                    }
                }else{
                    S->GetInput();
                    S->UnLock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                }
            }
        }
    }
}


#endif // PORT_HPP_INCLUDED
