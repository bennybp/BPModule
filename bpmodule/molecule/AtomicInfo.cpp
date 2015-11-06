#include "bpmodule/molecule/AtomicInfo.hpp"
#include "bpmodule/molecule/AtomicInfo_LUT.hpp"
#include "bpmodule/exception/MoleculeException.hpp"

namespace bpmodule {
namespace molecule {

// global LUT:
// Maps Z to symbol
using lut::atomic_Z_sym_;

// Maps symbol to Z
using lut::atomic_sym_Z_;

// Maps Z to data
using lut::atomic_Z_data_;


int AtomicZNumberFromSym(const std::string & sym)
{
    if(atomic_sym_Z_.count(sym))
        return atomic_sym_Z_.at(sym);
    else
        throw exception::MoleculeException("No Z number for this atomic symbol",
                                           "symbol", sym);
}



std::string AtomicSymFromZ(int Z)
{
    if(atomic_Z_sym_.count(Z))
        return atomic_Z_sym_.at(Z);
    else
        throw exception::MoleculeException("No symbol for this Z number",
                                           "Z", Z);
}



const AtomicData & AtomicInfoFromZ(int Z)
{
    if(atomic_Z_data_.count(Z))
        return atomic_Z_data_.at(Z);
    else
        throw exception::MoleculeException("No atomic data for this Z number",
                                           "Z", Z);
}



const AtomicData & AtomicInfoFromSym(const std::string & sym)
{
    return AtomicInfoFromZ(AtomicZNumberFromSym(sym));
}



const IsotopeData & IsotopeInfoFromZ(int Z, int isonum)
{
    AtomicData ad = AtomicInfoFromZ(Z);
    
    for(const auto & it : ad.isotopes)
        if(it.isonum == isonum)
            return it;

    throw exception::MoleculeException("No isotope data for this Z and isotope number",
                                           "Z", Z,
                                           "isotope", isonum);
}



const IsotopeData & IsotopeInfoFromSym(const std::string & sym, int isonum)
{
    return IsotopeInfoFromZ(AtomicZNumberFromSym(sym), isonum); 
}



double AtomicMassFromZ(int Z)
{
    return AtomicInfoFromZ(Z).mass;
}



double AtomicMassFromSym(const std::string & sym)
{
    return AtomicInfoFromSym(sym).mass;
}



double IsotopeMassFromZ(int Z, int isonum)
{
    return IsotopeInfoFromZ(Z, isonum).mass;
}



double IsotopeMassFromSym(const std::string & sym, int isonum)
{
    return IsotopeInfoFromSym(sym, isonum).mass;
}



std::string AtomicNameFromZ(int Z)
{
    return AtomicInfoFromZ(Z).name;
}



std::string AtomicNameFromSym(const std::string & sym)
{
    return AtomicInfoFromSym(sym).name;
}



} // closing namespace molecule
} // closing namespace bpmodule

