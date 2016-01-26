#include <sstream>
#include "bpmodule/system/Atom.hpp"
#include "bpmodule/system/AtomicInfo.hpp"

namespace bpmodule {
namespace system {

size_t DefaultMult(size_t Z){
    switch(Z){
        case 0 :{return 1;}
        case 1:{return 2;}
        case 2:{return 1;}
        case 3:{return 2;}
        case 4:{return 1;}
        case 5:{return 2;}
        case 6:{return 3;}
        case 7:{return 4;}
        case 8:{return 3;}
        case 9:{return 2;}
        case 10:{return 1;}
        default:{return 0;}
    }
}    

std::string Atom::Symbol() const{
    return AtomicNameFromZ((int)Z());
}

Atom::Atom(size_t ZIn,double x,double y,double z):
    Base_t(x,y,z){
        AddWeight("Z",(double)ZIn);
        SetMass(AtomicMassFromZ((int)ZIn));
        SetChargeAndMult(0.0,DefaultMult((int)ZIn));
        SetNElec((double)ZIn);
}

std::string Atom::ToString() const{
    std::stringstream ss;
    ss<<Symbol()<<" "<<(*this)[0]<<" "<<(*this)[1]<<" "<<(*this)[2];
    return ss.str();
}

    
/*double Atom::Mass(void) const
{
    return IsotopeMassFromZ(z, isonum);
}

std::string Atom::Name(void) const
{
    return AtomicNameFromZ(z);

}

std::string Atom::Symbol(void) const
{
    return AtomicSymFromZ(z);
}*/


} // close namespace system
} // close namespace bpmodule

