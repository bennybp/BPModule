#ifndef MODULEBASE_H
#define MODULEBASE_H

#include <string>
#include <functional>

#include "BPModule/core/BoostPython_fwd.hpp"
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

        
        template<typename T>
        T GetTrait(const std::string & key) const
        {
            return traits_.Get<T>(key);
        }

        template<typename T>
        T GetOption(const std::string & key) const
        {
            return options_.Get<T>(key);
        }

        template<typename T>
        void ChangeTrait(const std::string & key, const T & value)
        {
            traits_.Change(key, value);
        }

        template<typename T>
        void ChangeOption(const std::string & key, const T & value)
        {
            options_.Change(key, value);
        }



        // For use from python classes derived from this
        ModuleStore & MStore(void);
        


    protected:

    private:
        unsigned long id_;
        std::string key_;
        std::string name_;
        std::string version_;
        OptionMap options_;

        OptionMap traits_;

        ModuleStore & mstore_;
};

// Specializations for use from python
template<> boost::python::object ModuleBase::GetTrait<>(const std::string & key) const;
template<> boost::python::object ModuleBase::GetOption<>(const std::string & key) const;
template<> void ModuleBase::ChangeTrait<>(const std::string & key, const boost::python::object & value);
template<> void ModuleBase::ChangeOption<>(const std::string & key, const boost::python::object & value);


} // close namespace bpmodule

#endif
