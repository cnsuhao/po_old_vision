#ifndef PO_H
#define PO_H
#include "pocontext.h"
#include "plugin.h"
#include "render_packet.hpp"
#include <ostream>
namespace PO
{
    template<typename Tem>
        int Loop(const Tem& Func)
        {
            POContext POC;
            Func(POC);
            return POC.ContextLoop();
        }
    template<typename Tem>
        int Loop(std::ostream& OS,const Tem& Func)
        {
            POContext POC;
            try{
                Func(POC);
                return POC.ContextLoop();
            }catch(EnvironmentalError& EE)
            {
                OS<<EE.what();
            }catch(ResourceUnavailable& SU)
            {
                OS<<SU.what()<<" For Source In :<"<<SU.Source<<" >"<<std::endl;
            }catch(std::exception& L)
            {
                OS<<L.what()<<std::endl;
            }
            return 0;
        }
        int Loop(std::ostream& OS,void (*FP)(POContext&))
        {
            POContext POC;
            try{
                (*FP)(POC);
                return POC.ContextLoop();
            }catch(EnvironmentalError& EE)
            {
                OS<<EE.what();
            }catch(ResourceUnavailable& SU)
            {
                OS<<SU.what()<<" For Source In :<"<<SU.Source<<" >"<<std::endl;
            }catch(std::exception& L)
            {
                OS<<L.what()<<std::endl;
            }
            return 0;
        }
}
#endif // PO_HPP
