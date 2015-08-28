#ifndef MODULESTORE_H
#define MODULESTORE_H

#include <unordered_map>
#include <vector>
#include <string>
#include <atomic>

#include "BPModule/core/ModuleInfo.hpp"
#include "BPModule/python_helper/BoostPython_fwd.hpp"
namespace bpy = boost::python;


namespace bpmodule {


class ModuleBase;
class ModuleStore;


template<typename T>
using ScopedModule = std::unique_ptr<T, std::function<void(ModuleBase *)>>;



class ModuleStore
{
    public:
        typedef std::function<ModuleBase *(const std::string &, unsigned long, const ModuleInfo &)> ModuleGeneratorFunc;
        typedef std::function<void(unsigned long)> ModuleRemoverFunc;

        ModuleStore();
        ~ModuleStore();

        ModuleStore & operator=(const ModuleStore & rhs) = delete;
        ModuleStore(const ModuleStore & rhs) = delete;

        size_t Size(void) const;
        std::vector<std::string> GetKeys(void) const;
        ModuleInfo KeyInfo(const std::string & key) const;
        bool Has(const std::string & key) const;


        void AddModule(const std::string & key, ModuleGeneratorFunc func, ModuleRemoverFunc dfunc, const ModuleInfo & minfo);
        void RemoveModule(unsigned long id);
        void RemoveModule(ModuleBase * mb);

        void SetOptions(const std::string & key, const OptionMap & opt);



        template<typename T>
        T & GetModule(const std::string & key)
        {
            // obtain the creator
            const StoreEntry & se = GetOrThrow_(key);

            // create
            ModuleBase * mbptr = se.func(key, curid_, se.mi);

            // test
            T * dptr = dynamic_cast<T *>(mbptr);
            if(dptr == nullptr)
                throw BPModuleException(
                                 "Bad cast for module",
                                 {
                                     { "Location", "ModuleStore"},
                                     { "Key", key },
                                     { "From", typeid(mbptr).name() },
                                     { "To", typeid(T *).name() }
                                 }
                               );


            // store the deleter
            removemap_.insert(RemoverMap::value_type(curid_, se.dfunc));


            // next id
            curid_++;

            return *dptr;
        }


        template<typename T>
        ScopedModule<T> GetScopedModule(const std::string & key)
        {
            T & mod = GetModule<T>(key);
            std::function<void(ModuleBase *)> dfunc = std::bind(static_cast<void(ModuleStore::*)(ModuleBase *)>(&ModuleStore::RemoveModule),
                                                                this,
                                                                std::placeholders::_1);

            return ScopedModule<T>(&mod, dfunc);
        }



    private:

        struct StoreEntry
        {
            ModuleInfo mi;
            ModuleGeneratorFunc func;
            ModuleRemoverFunc dfunc;
        };

        typedef std::unordered_map<std::string, StoreEntry> StoreMap;
        typedef std::unordered_map<unsigned long, ModuleRemoverFunc> RemoverMap;

        StoreMap store_;
        RemoverMap removemap_;
        std::atomic<unsigned long> curid_;

        const StoreEntry & GetOrThrow_(const std::string & key) const;
};






namespace export_python {
void Wrap_ModuleStore_SetOptions(ModuleStore * mst, const std::string & key, bpy::list & opt);
}


} // close namespace bpmodule


#endif
