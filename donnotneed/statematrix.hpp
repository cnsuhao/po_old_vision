#ifndef STATEMACHINE_HPP_INCLUDED
#define STATEMACHINE_HPP_INCLUDED
#include <map>

template<typename State,typename Event>
struct StateMatrix
{
    StateMatrix():StartState(0),NowState(0){}
    StateMatrix(const State& I):StartState(I),NowState(I){}
    StateMatrix(const StateMatrix& K):Matrix(K.Matrix),StartState(K.StartState),NowState(K.NowState){}
    const State GetState()const {return NowState;}
    operator const State()const {return GetState();}
    void ResetState() {NowState=StartState;}
    void InsertRelation(const State& S,const State& U,const Event& E){Matrix[S][E]=U;}
    void InsertRelation(const State& S,const State& U,const Event*,int Size);
    bool ChangeState(const Event& K);
private:
    std::map<State,std::map<Event,State> > Matrix;
    State StartState;
    State NowState;
};

template<typename State,typename Event>
    void StateMatrix<State,Event>::InsertRelation(const State& S,const State& U,const Event* E,int Size)
    {
        auto& Map=Matrix[S];
        for(int i=0;i<Size;++E)
        {
            Map[*E]=U;
        }
    }

template<typename State,typename Event>
    bool StateMatrix<State,Event>::ChangeState(const Event& K)
    {
        auto& Tem=Matrix[NowState];
        auto Oi=Tem.find(K);
        if(Oi!=Tem.end())
        {
            NowState=Oi->second;
            return true;
        }
        return false;
    }
#endif // STATEMACHINE_HPP_INCLUDED
