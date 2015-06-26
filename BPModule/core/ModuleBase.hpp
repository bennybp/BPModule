#ifndef MODULEBASE_H
#define MODULEBASE_H

#include <string>
#include <functional>

#include "BPModule/core/ModuleInfo.hpp"

namespace bpmodule {

class ModuleStore;


class ModuleBase
{
    public:
        ModuleBase(unsigned long id, ModuleStore & mstore, const ModuleInfo & minfo);

        virtual ~ModuleBase();

        ModuleBase(const ModuleBase & rhs) = delete;
        ModuleBase(ModuleBase && rhs) = delete;
        ModuleBase & operator= (const ModuleBase & rhs) = delete;
        ModuleBase & operator= (ModuleBase && rhs) = delete;

        unsigned long ID(void) const;
        const std::string & Key(void) const;
        const std::string & Name(void) const;
        const std::string & Version(void) const;

        OptionMap Traits(void) const;
        OptionMap Options(void) const;
        ModuleStore & MStore(void);

    protected:
        template<typename T>
        T GetTrait(const std::string & key) const
        {
            return traits_.Get<T>(key);
        }

        template<typename T>
        void SetTrait(const std::string & key, const T & value)
        {
            traits_.Set(key, value);
        }

        template<typename T>
        T GetOption(const std::string & key) const
        {
            return options_.Get<T>(key);
        }

        template<typename T>
        void SetOption(const std::string & key, const T & value)
        {
            options_.Set(key, value);
        }


    private:
        unsigned long id_;
        std::string key_;
        std::string name_;
        std::string version_;
        OptionMap options_;

        OptionMap traits_;

        ModuleStore & mstore_;
};


} // close namespace bpmodule

#endif
