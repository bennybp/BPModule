/*! \file
 *
 * \brief Structure storing information about a module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULEMANAGER__MODULEINFO_HPP_
#define BPMODULE_GUARD_MODULEMANAGER__MODULEINFO_HPP_


#include "bpmodule/datastore/OptionMap.hpp"


namespace bpmodule {
namespace modulemanager {

/*! \brief Structure that stores information about a module
 */
struct ModuleInfo
{
    std::string name;                  //!< Some descriptive name for the module
    std::string type;                  //!< Type of the supermodule (c_module, python_module, etc)
    std::string base;                  //!< Base class of this module (EnergyMethod, etc)
    std::string path;                  //!< Full path for the supermodule (including the filename for an SO file)
    std::string version;               //!< Arbitrary version string of the supermodule
    std::string description;           //!< A description of the module
    std::vector<std::string> authors;  //!< Authors of the module
    std::vector<std::string> refs;     //!< References for the module (articles, web pages, etc)
    datastore::OptionMap options;      //!< Options for the module


    /*! \brief Print out this info
    */
    void Print(std::ostream & os) const;
};


} // close namespace modulemanager
} // close namespace bpmodule


#endif
