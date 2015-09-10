/*! \file
 *
 * \brief Loading and storing of Python modules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_PYMODULELOADER_HPP_
#define _GUARD_PYMODULELOADER_HPP_

#include <memory>
#include <unordered_map>
#include <string>

#include <boost/python.hpp>


//forward declarations
namespace bpmodule {

namespace modulebase {
class ModuleBase;
}

namespace modulestore {
class ModuleStore;
class ModuleInfo;
}
}
// end forward declarations




namespace bpmodule {
namespace modulestore {

class PyModuleLoader
{
    public:

        PyModuleLoader(modulestore::ModuleStore * mst);
        ~PyModuleLoader();

        PyModuleLoader & operator=(const PyModuleLoader & rhs) = delete;
        PyModuleLoader(const PyModuleLoader & rhs) = delete;

        void AddPyModule(const std::string & key, boost::python::object func, const modulestore::ModuleInfo & minfo);

        void UnloadAll(void);

    private:
        typedef std::unordered_map<unsigned long, boost::python::object> ObjectMap;

        modulestore::ModuleStore * mst_;
        ObjectMap objects_;

        void DeleteObject_(unsigned long id);

        modulebase::ModuleBase * CreateWrapper_(boost::python::object fn,
                                                const std::string & key,
                                                unsigned long id,
                                                ModuleStore & mstore,
                                                const ModuleInfo & minfo);

        void DeleteWrapper_(unsigned long id);

};


} // close namespace modulestore
} // close namespace bpmodule


#endif
