#ifndef CMODULELOADER_H
#define CMODULELOADER_H

#include <memory>
#include <unordered_map>
#include <string>

namespace bpmodule {


class ModuleBase;
class ModuleStore;
class ModuleInfo;


class CModuleLoader
{
    public:
        CModuleLoader(ModuleStore * mst);
        ~CModuleLoader();

        CModuleLoader & operator=(const CModuleLoader & rhs) = delete;
        CModuleLoader(const CModuleLoader & rhs) = delete;

        bool LoadSO(const std::string & key, const ModuleInfo & minfo);

        void DeleteAll(void);
        void CloseHandles(void);

    private:
        typedef ModuleBase *(*CreateFunc)(const std::string &, unsigned long, ModuleStore &, const ModuleInfo &);

        typedef std::unordered_map<std::string, void *> HandleMap;
        typedef std::unordered_map<unsigned long, std::unique_ptr<ModuleBase>> ObjectMap;

        ModuleStore * mst_;
        HandleMap handles_;
        ObjectMap objects_;

        void DeleteObject_(unsigned long id);

        ModuleBase * CreateWrapper_(CreateFunc fn, const std::string & key, unsigned long id, const ModuleInfo & minfo);
        void DeleteWrapper_(unsigned long id);
};

} // close namespace bpmodule


#endif
