#ifndef PYMODULELOADER_H
#define PYMODULELOADER_H

#include <memory>
#include <unordered_map>
#include <string>

#include <boost/python.hpp>

namespace bpmodule {


class ModuleBase;
class ModuleStore;
class ModuleInfo;


class PyModuleLoader
{
    public:

        PyModuleLoader(ModuleStore * mst);
        ~PyModuleLoader();

        PyModuleLoader & operator=(const PyModuleLoader & rhs) = delete;
        PyModuleLoader(const PyModuleLoader & rhs) = delete;

        void AddPyModule(const std::string & key, boost::python::object func, const ModuleInfo & minfo);

        void DeleteAll(void);

    private:
        typedef std::unordered_map<unsigned long, boost::python::object> ObjectMap;

        ModuleStore * mst_;
        ObjectMap objects_;

        void DeleteObject_(unsigned long id);

        ModuleBase * CreateWrapper_(boost::python::object fn, const std::string & key, unsigned long id, const ModuleInfo & options);
        void DeleteWrapper_(unsigned long id);
};

} // close namespace bpmodule


#endif
