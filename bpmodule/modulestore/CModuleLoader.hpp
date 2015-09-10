/*! \file
 *
 * \brief Loading and storing of C modules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_CMODULELOADER_HPP_
#define _GUARD_CMODULELOADER_HPP_

#include <memory>
#include <unordered_map>
#include <string>


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

        modulebase::ModuleBase * CreateWrapper_(CreateFunc fn,
                                                const std::string & key,
                                                unsigned long id,
                                                ModuleStore & mstore,
                                                const ModuleInfo & minfo);

        void DeleteWrapper_(unsigned long id);

};

} // close namespace modulestore
} // close namespace bpmodule


#endif
