/*! \file
 *
 * \brief Structure storing information about a module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_MODULEINFO_HPP_
#define _GUARD_MODULEINFO_HPP_


#include "bpmodule/python_helper/BoostPython_fwd.hpp"
#include "bpmodule/datastore/OptionMap.hpp"


namespace bpmodule {
namespace modulestore {

/*! \brief Structure that stores information about a loaded module
 */
struct ModuleInfo
{
    std::string key;
    std::string name;
    std::string type;
    std::string path;
    std::string soname;
    std::string version;
    std::vector<std::string> authors;
    std::string description;
    std::vector<std::string> refs;
    datastore::OptionMap options;

    ModuleInfo(void) = default;
    ModuleInfo(const ModuleInfo & rhs) = default;
    ModuleInfo(ModuleInfo && rhs) = default;
    ~ModuleInfo(void) = default;

    /*! \brief Construct from a python dictionary
     * 
     * \todo Document or wrap boost python exceptions
     *
     * \throw bpmodule::exception::GeneralException if there is
     *        a problem (conversion problem, missing keys, etc)
     */
    ModuleInfo(const boost::python::dict & dictionary);
};


} // close namespace modulestore
} // close namespace bpmodule


#endif
