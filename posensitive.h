#ifndef POSENSITIVE_HPP_INCLUDED
#define POSENSITIVE_HPP_INCLUDED
#include <functional>
#include <map>
#include <set>
#include <memory>
#include "elementfigure.h"
namespace PO
{
    class MouseSensitive
    {
        struct MouseBase
        {
            float CX,CY;
            virtual bool RunPacket(float X,float Y)=0;
            virtual ~MouseBase(){}
            MouseBase(float X,float Y):CX(X),CY(Y){}
        };
        template <typename Packet>
            struct Mouse:public MouseBase
            {
                Packet Data;Mouse(float X,float Y,const Packet& D):MouseBase(X,Y),Data(D){}
                virtual bool RunPacket(float X,float Y){return Data(X,Y);}
                Mouse(float X,float Y):MouseBase(X,Y){}
            };
        unsigned int Count;
        std::map<unsigned int ,std::shared_ptr<MouseBase>> AllMouse;
    public:
        template<typename Type>
            void Insert(unsigned int& ID,float X,float Y,const Type& T);
        template<typename ..._FunP>
            void Insert(unsigned int& ID,float X,float Y,bool (*FP)(float X,float Y,_FunP...),_FunP... _par)
                {Insert(ID,X,Y,std::bind( FP,std::placeholders::_1,std::placeholders::_2,_par... )); }
        template<typename Type>
            void Insert(unsigned int& ID,const Vertex<2>& L,const Type& T){Insert(ID,L(0),L(1),T);}
        template<typename ..._FunP>
            void Insert(unsigned int& ID,const Vertex<2>& L,bool (*FP)(float X,float Y,_FunP...),_FunP... _par){Insert(ID,L(0),L(1),FP,_par...);}

        bool GetLocation(unsigned int ID,float& X,float& Y);
        bool GetLocation(unsigned int ID,Vertex<2>& Y){return GetLocation(ID,Y(0),Y(1));}

        template<template<typename Go,typename ...Ty> class Tank >
            bool Respond(float MX,float MY,Tank<unsigned int>& TT);
        template<template<typename Go,typename ...Ty> class Tank >
            bool Respond(const Vertex<2>& L,Tank<unsigned int>& TT){return Respond(L(0),L(1),TT);}
        bool SingleRespond(float MX,float MY,unsigned int K);
        bool SingleRespond(const Vertex<2>& L,unsigned int K){return SingleRespond(L(0),L(1),K);}

        bool Respond(float MX,float MY,std::set<unsigned int>& TT);
        bool Respond(const Vertex<2>& MY,std::set<unsigned int>& TT){return Respond(MY(0),MY(1),TT);}

        bool Change(unsigned int ID,float X,float Y);
        bool Change(unsigned int ID,const Vertex<2>& Y){return Change(ID,Y(0),Y(1));}
        bool Clear(unsigned int ID);
        void Clear(){AllMouse.clear();}
    };

    template<template<typename Go,typename ...Ty> class Tank >
        bool MouseSensitive::Respond(float MX,float MY,Tank<unsigned int>& TT)
        {
            bool Find=false;
            for(auto& I:AllMouse)
            {
                if((I.second)->RunPacket(MX-(I.second)->CX,MY-(I.second)->CY))
                {
                    Find=true;
                    TT.push_back(I.first);
                }
            }
            return Find;
        }

    template<typename Type>
        void MouseSensitive::Insert(unsigned int& ID,float X,float Y,const Type& T)
        {
            std::shared_ptr<MouseBase> Tem(new Mouse<Type>(X,Y,T));
            Count+=1;
            ID=Count;
            AllMouse.insert(std::make_pair(ID,Tem));
        }

}


#endif // POSENSITIVE_HPP_INCLUDED
