#ifndef WINSERIALPORT_H
#define WINSERIALPORT_H
#include <string>
#include "windows.h"
class WinSerialPort
{
    HANDLE hCom;
    DCB DCBPoint;
    COMMTIMEOUTS TimeOutPoint;
    bool Open;
    std::string Name;
    COMSTAT State;
public:
    operator bool() const{return Open;}
    bool IsOpen()const {return Open;}
    bool CreatSerial(const std::string& N,unsigned int Bit=9600);
    const std::string& GetName()const {return Name;}
    bool CloseSerial();
    WinSerialPort():Open(false) {}
    bool WriteFinish();
    bool ReadSomeThing();
    bool Write(char* data,DWORD Size,DWORD& WS)
    {
        return WriteFile(hCom,data,Size,&WS,NULL);
    }
    bool ScanfInput(char* data,DWORD Size,DWORD& RS)
    {
        return ReadFile(hCom,data,Size,&RS,NULL);
    }
    bool GetState(DWORD& K)
    {
        return ClearCommError(hCom,&K,&State);
    }
    unsigned int ReadBuffer()
    {
        return State.cbInQue;
    }
    unsigned int WriteBuffer()
    {
        return State.cbOutQue;
    }
    ~WinSerialPort()
    {
        CloseSerial();
    }
};
bool WinSerialPort::CloseSerial()
{
    if(Open)
    {
        CloseHandle(hCom);
        Open=false;
        return true;
    }
    return false;
}
bool WinSerialPort::CreatSerial(const std::string& Tr,unsigned int Bit)
{
    CloseSerial();
    hCom=CreateFile(Tr.c_str(),GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL/*|FILE_FLAG_OVERLAPPED*/,NULL);
    if(hCom==INVALID_HANDLE_VALUE)
        return false;
    Open=true;
    Name=Tr;
    if(!SetupComm(hCom,1024,1024))
    {
        CloseSerial();
        return false;
    }
    if(!GetCommState(hCom,&DCBPoint))
    {
        CloseSerial();
        return false;
    }
    DCBPoint.ByteSize=8;
    DCBPoint.Parity=NOPARITY;
    DCBPoint.StopBits=ONESTOPBIT;
    DCBPoint.BaudRate=Bit;
    SetCommMask(hCom,EV_RXCHAR);
    if(!SetCommState(hCom,&DCBPoint))
    {
        CloseSerial();
        return false;
    }
    if(!GetCommTimeouts(hCom,&TimeOutPoint))
    {
        CloseSerial();
        return false;
    }
    TimeOutPoint.ReadIntervalTimeout=10; //�������ʱ
    TimeOutPoint.ReadTotalTimeoutMultiplier=50; //��ʱ��ϵ��
    TimeOutPoint.ReadTotalTimeoutConstant=50; //��ʱ�䳣��
    TimeOutPoint.WriteTotalTimeoutMultiplier=50; // дʱ��ϵ��
    TimeOutPoint.WriteTotalTimeoutConstant=10; //дʱ�䳣��
    if(!SetCommTimeouts(hCom,&TimeOutPoint))
    {
        CloseSerial();
        return false;
    }
    return true;
}

#endif // WINSERIALPORT_H
