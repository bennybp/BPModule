/*! \file
 *
 * \brief Structure storing information about a module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_MODULEINFO_HPP_
#define _GUARD_MODULEINFO_HPP_


#include "bpmodule/datastore/OptionMap.hpp"


namespace bpmodule {
namespace modulestore {

/*! \brief Structure that stores information about a loaded module
 */
struct ModuleInfo
{
    std::string key;                   //!< The key in the module database
    std::string name;                  //!< Some descriptive name for the module
    std::string type;                  //!< Type of the module (c_module, python_module, etc)
    std::string path;                  //!< Path for the module (not including the filename for an so file)
    std::string soname;                //!< SO filename (for c modules)
    std::string version;               //!< Arbitrary version string
    std::vector<std::string> authors;  //!< Authors of the module
    std::string description;           //!< A description of the module
    std::vector<std::string> refs;     //!< References for the module (articles, web pages, etc)
    datastore::OptionMap options;      //!< Options for the module


    ModuleInfo(void)                    = default;
    ModuleInfo(const ModuleInfo & rhs)  = default;
    ModuleInfo(ModuleInfo && rhs)       = default;
    ~ModuleInfo(void)                   = default;


    /*! \brief Construct from a python dictionary
     * 
     * \throw bpmodule::exception::PythonConvertException if conversion fails or the
     *        dictionary doesn't contain the right data (missing keys, etc) 
     */
    ModuleInfo(const boost::python::dict & dictionary);
};


} // close namespace modulestore
} // close namespace bpmodule


#endif
