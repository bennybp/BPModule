#ifndef MODULESTORE_H
#define MODULESTORE_H

#include <unordered_map>
#include <vector>
#include <string>
#include <atomic>

#include "BPModule/core/ModuleInfo.hpp"

namespace bpmodule {


class ModuleBase;
class ModuleStore;


template<typename T>
using ScopedModule = std::unique_ptr<T, std::function<void(ModuleBase *)>>;



class ModuleStore
{
    public:
        typedef std::function<ModuleBase *(const std::string &, unsigned long, const ModuleInfo &)> ModuleGeneratorFunc;
        typedef std::function<void(unsigned long)> ModuleDeleterFunc;

        ModuleStore();
        ~ModuleStore();

        ModuleStore & operator=(const ModuleStore & rhs) = delete;
        ModuleStore(const ModuleStore & rhs) = delete;

        void Lock(void);
        bool AddCreateFunc(const std::string & key, ModuleGeneratorFunc func, ModuleDeleterFunc dfunc, const ModuleInfo & minfo);

        size_t Size(void) const;
        std::vector<std::string> GetKeys(void) const;
        ModuleInfo ModuleInfoFromKey(const std::string & key) const;
        bool Has(const std::string & key) const;

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
                throw MapException("ModuleStore", key, typeid(mbptr).name(), typeid(T *).name());

            // store the deleter
            deletemap_.insert(DeleterMap::value_type(curid_, se.dfunc));


            // next id
            curid_++;

            return *dptr;
        }


        template<typename T>
        ScopedModule<T> GetScopedModule(const std::string & key)
        {
            T & mod = GetModule<T>(key);
            std::function<void(ModuleBase *)> dfunc = std::bind(static_cast<void(ModuleStore::*)(ModuleBase *)>(&ModuleStore::Delete), this, std::placeholders::_1);
            return ScopedModule<T>(&mod, dfunc);
        }

        void Delete(unsigned long id);

        void Delete(ModuleBase * mb);


    private:

        struct StoreEntry
        {
            ModuleInfo mi;
            ModuleGeneratorFunc func;
            ModuleDeleterFunc dfunc;
        };

        typedef std::unordered_map<std::string, StoreEntry> StoreMap;
        typedef std::unordered_map<unsigned long, ModuleDeleterFunc> DeleterMap;

        StoreMap store_;
        DeleterMap deletemap_;
        std::atomic<unsigned long> curid_;

        bool locked_;

        const StoreEntry & GetOrThrow_(const std::string & key) const;
};

} // close namespace bpmodule


#endif
