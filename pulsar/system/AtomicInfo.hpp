/*\file
 *
 * \brief Functionality for getting experimental/empirical atomic data (header)
*/


#ifndef PULSAR_GUARD_SYSTEM__ATOMICINFO_HPP_
#define PULSAR_GUARD_SYSTEM__ATOMICINFO_HPP_

#include <vector>
#include <string>

namespace pulsar{
namespace system {

/*! \brief Information about a particular elemental isotope */
struct IsotopeData
{
    int isonum;           //! Isotope number (Z + number of neutrons)
    double mass;          //! Mass of the isotope
    double mass_low;      //! Lower bound of the isotope mass
    double mass_high;     //! Upper bound of the isotope mass

    double abund;         //! Natural abundance of the isotope (out of 1)
    double abund_low;     //! Lower bound on the isotope's abundance
    double abund_high;    //! Upper bound on the isotope's abundance 
};


/*! \brief Information about an atom/element */
struct AtomicData
{
    int Z;                //! Atomic Z-number (number of protons)
    std::string sym;      //! Element's atomic symbol
    std::string name;     //! Full name of the element 
    int multiplicity;     //! Ground-state multiplicity
    std::string termsym;  //! Term symbol character
    double mass;          //! Mass of the element (isotope masses weighted by abundance)
    double mass_low;      //! Lower bound on the mass of the element
    double mass_high;     //! Upper bound on the mass of the element
    double covradius;     //! Covalent radius in a.u.
    double vdwradius;     //! VDW radius in a.u.
    
    std::vector<IsotopeData> isotopes; //!< All isotope information for this atom
};


//////////////////////////
// Free functions
//////////////////////////

/*! \brief Get atomic data by Z number
 *
 * \throw pulsar::exception::SystemException if the
 *        data does not exist for this Z number
 */
const AtomicData & atomic_info_from_z(int Z);


/*! \brief Get atomic data by symbol
 *
 * Symbol is not case sensitive
 *
 * \throw pulsar::exception::SystemException if the
 *        data does not exist for this symbol
 */
const AtomicData & atomic_info_from_symbol(const std::string & sym);



/*! \brief Get isotope data by Z number and isotope number
 *
 * \throw pulsar::exception::SystemException if the
 *        data does not exist for this Z number and isotope number.
 */
const IsotopeData & isotope_info_from_z(int Z, int isonum);



/*! \brief Get isotope data by symbol and isotope number
 *
 * Symbol is not case sensitive
 *
 * \throw pulsar::exception::SystemException if the
 *        data does not exist for this symbol and isotope number.
 */
const IsotopeData & isotope_info_from_symbol(const std::string & sym, int isonum);



/*! \brief Get the most common isotope by Z number
 *
 * \throw pulsar::exception::SystemException if the
 *        data does not exist for this Z number and isotope number.
 */
int most_common_isotope_from_z(int Z); 



/*! \brief Get the most common isotope by symbol
 *
 * Symbol is not case sensitive
 *
 * \throw pulsar::exception::SystemException if the
 *        data does not exist for this symbol and isotope number.
 */
int most_common_isotope_from_symbol(const std::string & sym); 



/*! \brief Look up an atomic mass by Z number
 *
 * \throw pulsar::exception::SystemException if the
 *        data does not exist for this Z number
 */
double atomic_mass_from_z(int Z);



/*! \brief Look up an atomic mass by symbol
 *
 * Symbol is not case sensitive
 *
 * \throw pulsar::exception::SystemException if the
 *        data does not exist for this symbol
 */
double atomic_mass_from_symbol(const std::string & sym);



/*! \brief Look up an isotope mass by Z number and isotope number
 *
 * \throw pulsar::exception::SystemException if the
 *        data does not exist for this Z number and isotope number.
 */
double isotope_mass_from_z(int Z, int isonum);



/*! \brief Look up an isotope mass by symbol and isotope number
 *
 * Symbol is not case sensitive
 *
 * \throw pulsar::exception::SystemException if the
 *        data does not exist for this symbol and isotope number
 */
double isotope_mass_from_symbol(const std::string & sym, int isonum);



/*! \brief Look up a atomic symbol by Z number 
 *
 *  \throw pulsar::exception::SystemException if the
 *         data does not exist for this symbol
 */
std::string atomic_symbol_from_z(int Z);



/*! \brief Look up Z number by symbol
 *
 * Symbol is not case sensitive
 *
 * \throw pulsar::exception::SystemException if the
 *        symbol is not defined for this Z number
 */
int atomic_z_from_symbol(const std::string & sym);



/*! Look up an atomic name by Z number
 *
 * \throw pulsar::exception::SystemException if the
 *        name is not defined for this Z number
 */
std::string atomic_name_from_z(int Z);



/*! \brief Look up an atomic name by symbol
 *
 * Symbol is not case sensitive
 *
 * \throw pulsar::exception::SystemException if the
 *        name is not defined for this symbol
 */
std::string atomic_name_from_symbol(const std::string & sym);



/*! \brief Look up an atomic ground-state multiplicity by Z number
 *
 * \throw pulsar::exception::SystemException if the
 *        name is not defined for this Z number
 */
int atomic_multiplicity_from_z(int Z);



/*! \brief Look up an atomic ground-state multiplicity by symbol
 *
 * Symbol is not case sensitive
 *
 * \throw pulsar::exception::SystemException if the
 *        name is not defined for this symbol
 */
int atomic_multiplicity_from_symbol(const std::string & sym);


/*! \brief Look up the covalent radius by Z number
 *
 * \throw pulsar::exception::SystemException if the
 *        name is not defined for this symbol
 */
double covalent_radius_from_z(int Z);


/*! \brief Look up the covalent radius by symbol
 *
 * Symbol is not case sensitive
 *
 * \throw pulsar::exception::SystemException if the
 *        name is not defined for this symbol
 */
double covalent_radius_from_symbol(const std::string & sym);

/*! \brief Look up the Van der Waals radius by Z number
 *
 * \throw pulsar::exception::SystemException if the
 *        name is not defined for this symbol
 */
double vdw_radius_from_z(int Z);


/*! \brief Look up the covalent radius by symbol
 *
 * Symbol is not case sensitive
 *
 * \throw pulsar::exception::SystemException if the
 *        name is not defined for this symbol
 */
double vdw_radius_from_symbol(const std::string & sym);




//! Z-number representing a ghost atom
static const int GHOST_ATOM_Z = 0;

//! Z-number representing a dummy atom
static const int DUMMY_ATOM_Z = -1;

//! Z-number representing a point charge
static const int POINTCHARGE_ATOM_Z = -2;



} // closing namespace system
} // closing namespace pulsar


#endif
