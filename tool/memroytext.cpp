#include "memorypool.h"
#include <iostream>
using namespace std;

class TexA
{
public:
    TexA(){cout<<"Construct"<<endl;}
    static void* operator new( size_t S){cout<<"Run new"<<endl;void* TC=new char;return TC;}
    static void operator delete (void* K){delete (K);cout<<"Run Delete"<<endl;}
};

int main()
{
    TexA* T=new TexA;
    delete T;
}
