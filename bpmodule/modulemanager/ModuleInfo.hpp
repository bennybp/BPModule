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

/*! \brief Structure that stores information about a loaded module
 */
struct ModuleInfo
{
    std::string name;                  //!< Some descriptive name for the module
    std::string type;                  //!< Type of the module (c_module, python_module, etc)
    std::string path;                  //!< Path for the module (not including the filename for an so file)
    std::string soname;                //!< SO filename (for c modules)
    std::string version;               //!< Arbitrary version string
    std::string description;           //!< A description of the module
    std::vector<std::string> authors;  //!< Authors of the module
    std::vector<std::string> refs;     //!< References for the module (articles, web pages, etc)
    datastore::OptionMap options;        //!< Options for the module


    /*! \brief Print out this info
    */
    void Print(void) const;
};


} // close namespace modulemanager
} // close namespace bpmodule


#endif
