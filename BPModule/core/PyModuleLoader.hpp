#ifndef PYMODULELOADER_H
#define PYMODULELOADER_H

#include <memory>
#include <unordered_map>
#include <string>

#include <boost/python.hpp>
namespace bpy = boost::python;



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

        void AddPyModule(const std::string & key, bpy::object func, const ModuleInfo & minfo);

        void DeleteAll(void);

    private:
        typedef std::unordered_map<unsigned long, bpy::object> ObjectMap;

        ModuleStore * mst_;
        ObjectMap objects_;

        void DeleteObject_(unsigned long id);

        ModuleBase * CreateWrapper_(bpy::object fn, const std::string & key, unsigned long id, const ModuleInfo & options);
        void DeleteWrapper_(unsigned long id);
};


namespace export_python {

void Wrap_PyModuleLoader_AddPyModule(PyModuleLoader * ml,
                                     const std::string & key, bpy::object func,
                                     const bpy::dict & minfo);

} // close namespace export_python


} // close namespace bpmodule


#endif
