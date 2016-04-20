/*\file
 *
 * \brief Functionality for getting experimental/empirical atomic data (source)
*/


#include <algorithm>

#include "bpmodule/system/AtomicInfo.hpp"
#include "bpmodule/system/AtomicInfo_LUT.hpp"
#include "bpmodule/exception/Exceptions.hpp"

namespace bpmodule {
namespace system {

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
        throw exception::SystemException("No Z number for this atomic symbol",
                                           "symbol", sym);
}



std::string AtomicSymFromZ(int Z)
{
    if(atomic_Z_sym_.count(Z))
        return atomic_Z_sym_.at(Z);
    else
        throw exception::SystemException("No symbol for this Z number",
                                           "Z", Z);
}



const AtomicData & AtomicInfoFromZ(int Z)
{
    if(atomic_Z_data_.count(Z))
        return atomic_Z_data_.at(Z);
    else
        throw exception::SystemException("No atomic data for this Z number",
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

    throw exception::SystemException("No isotope data for this Z and isotope number",
                                           "Z", Z,
                                           "isotope", isonum);
}



const IsotopeData & IsotopeInfoFromSym(const std::string & sym, int isonum)
{
    return IsotopeInfoFromZ(AtomicZNumberFromSym(sym), isonum); 
}


int MostCommonIsotopeFromZ(int Z)
{
    const AtomicData & ad = AtomicInfoFromZ(Z);
    if(ad.isotopes.size() == 0)
        throw exception::SystemException("Developer error: No isotopes for this Z number?",
                                           "Z", Z);

    auto maxit = std::max_element(ad.isotopes.begin(), ad.isotopes.end(),
                                  [](const IsotopeData & idat1, const IsotopeData & idat2)
                                    { return idat1.abund < idat2.abund; }
                                 );

    return maxit->isonum;
}

int MostCommonIsotopeFromSym(const std::string & sym)
{
    return MostCommonIsotopeFromZ(AtomicZNumberFromSym(sym));
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



int AtomicMultiplicityFromZ(int Z)
{
    return AtomicInfoFromZ(Z).multiplicity;
}



int AtomicMultiplicityFromSym(const std::string & sym)
{
    return AtomicInfoFromSym(sym).multiplicity;
}

double CovRadiusFromZ(int Z)
{
    return AtomicInfoFromZ(Z).covradius;
}

double CovRadiusFroSym(const std::string & sym)
{
    return AtomicInfoFromSym(sym).covradius;
}

double VDWRadiusFromZ(int Z)
{
    return AtomicInfoFromZ(Z).vdwradius;
}

double VDWRadiusFroSym(const std::string & sym)
{
    return AtomicInfoFromSym(sym).vdwradius;
}

} // closing namespace system
} // closing namespace bpmodule

