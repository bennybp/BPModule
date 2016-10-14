/*\file
 *
 * \brief Functionality for getting experimental/empirical atomic data (source)
*/


#include <algorithm>

#include "pulsar/system/AtomicInfo.hpp"
#include "pulsar/system/AtomicInfo_LUT.hpp"
#include "pulsar/exception/Exceptions.hpp"

namespace pulsar{
namespace system {

// global LUT:
// Maps Z to symbol
using lut::atomic_Z_sym_;

// Maps symbol to Z
using lut::atomic_sym_Z_;

// Maps Z to data
using lut::atomic_Z_data_;


int atomic_z_from_symbol(const std::string & sym)
{
    if(sym[0]=='@')
        return atomic_sym_Z_.at(sym.substr(1));
    if(atomic_sym_Z_.count(sym))
        return atomic_sym_Z_.at(sym);
    else
        throw SystemException("No Z number for this atomic symbol",
                                           "symbol", sym);
}



std::string atomic_symbol_from_z(int Z)
{
    if(Z<0)
      return "@"+atomic_symbol_from_z(abs(Z));
    if(atomic_Z_sym_.count(Z))
        return atomic_Z_sym_.at(Z);
    else
        throw SystemException("No symbol for this Z number",
                                           "Z", Z);
}



const AtomicData & atomic_info_from_z(int Z)
{
    if(Z<0)
        return atomic_Z_data_.at(GHOST_ATOM_Z);
    if(atomic_Z_data_.count(Z))
        return atomic_Z_data_.at(Z);
    else
        throw SystemException("No atomic data for this Z number",
                                           "Z", Z);
}



const AtomicData & atomic_info_from_symbol(const std::string & sym)
{
    return atomic_info_from_z(atomic_z_from_symbol(sym));
}



const IsotopeData & isotope_info_from_z(int Z, int isonum)
{
    const AtomicData & ad = atomic_info_from_z(Z);
    
    for(const auto & it : ad.isotopes)
        if(it.isonum == isonum)
            return it;

    throw SystemException("No isotope data for this Z and isotope number",
                                           "Z", Z,
                                           "isotope", isonum);
}



const IsotopeData & isotope_info_from_symbol(const std::string & sym, int isonum)
{
    return isotope_info_from_z(atomic_z_from_symbol(sym), isonum); 
}


int most_common_isotope_from_z(int Z)
{
    const AtomicData & ad = atomic_info_from_z(Z);
    if(ad.isotopes.size() == 0)
        throw SystemException("Developer error: No isotopes for this Z number?",
                                           "Z", Z);

    auto maxit = std::max_element(ad.isotopes.begin(), ad.isotopes.end(),
                                  [](const IsotopeData & idat1, const IsotopeData & idat2)
                                    { return idat1.abund < idat2.abund; }
                                 );

    return maxit->isonum;
}

int most_common_isotope_from_symbol(const std::string & sym)
{
    return most_common_isotope_from_z(atomic_z_from_symbol(sym));
}
 

double atomic_mass_from_z(int Z)
{
    return atomic_info_from_z(Z).mass;
}



double atomic_mass_from_symbol(const std::string & sym)
{
    return atomic_info_from_symbol(sym).mass;
}



double isotope_mass_from_z(int Z, int isonum)
{
    return isotope_info_from_z(Z, isonum).mass;
}



double isotope_mass_from_symbol(const std::string & sym, int isonum)
{
    return isotope_info_from_symbol(sym, isonum).mass;
}



std::string atomic_name_from_z(int Z)
{
    return atomic_info_from_z(Z).name;
}



std::string atomic_name_from_symbol(const std::string & sym)
{
    return atomic_info_from_symbol(sym).name;
}



int atomic_multiplicity_from_z(int Z)
{
    return atomic_info_from_z(Z).multiplicity;
}



int atomic_multiplicity_from_symbol(const std::string & sym)
{
    return atomic_info_from_symbol(sym).multiplicity;
}

double covalent_radius_from_z(int Z)
{
    return atomic_info_from_z(Z).covradius;
}

double covalent_radius_from_symbol(const std::string & sym)
{
    return atomic_info_from_symbol(sym).covradius;
}

double vdw_radius_from_z(int Z)
{
    return atomic_info_from_z(Z).vdwradius;
}

double vdw_radius_from_symbol(const std::string & sym)
{
    return atomic_info_from_symbol(sym).vdwradius;
}

} // closing namespace system
} // closing namespace pulsar

