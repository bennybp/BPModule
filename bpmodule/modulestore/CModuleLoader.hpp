#ifndef CMODULELOADER_H
#define CMODULELOADER_H

#include <memory>
#include <unordered_map>
#include <string>

#include "bpmodule/python_helper/BoostPython_fwd.hpp"

// Forward declarations
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


class CModuleLoader
{
    public:
        CModuleLoader(ModuleStore * mst);
        ~CModuleLoader();

        CModuleLoader & operator=(const CModuleLoader & rhs) = delete;
        CModuleLoader(const CModuleLoader & rhs) = delete;

        void LoadSO(const std::string & key, const ModuleInfo & minfo);

        void UnloadAll(void);
        void CloseHandles(void);

    private:
        typedef modulebase::ModuleBase *(*CreateFunc)(const std::string &, unsigned long, ModuleStore &, const ModuleInfo &);

        typedef std::unordered_map<std::string, void *> HandleMap;
        typedef std::unordered_map<unsigned long, std::unique_ptr<modulebase::ModuleBase>> ObjectMap;

        ModuleStore * mst_;
        HandleMap handles_;
        ObjectMap objects_;

        void DeleteObject_(unsigned long id);

        modulebase::ModuleBase * CreateWrapper_(CreateFunc fn, const std::string & key, unsigned long id, const ModuleInfo & minfo);
        void DeleteWrapper_(unsigned long id);
};


namespace export_python {

void Wrap_CModuleLoader_LoadSO(CModuleLoader * ml, const std::string & key, const boost::python::dict & minfo);

} // close namespace export_python


} // close namespace modulestore
} // close namespace bpmodule


#endif
