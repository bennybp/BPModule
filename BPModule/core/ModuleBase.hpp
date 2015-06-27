#ifndef MODULEBASE_H
#define MODULEBASE_H

#include <string>
#include <functional>

#include "BPModule/core/ModuleInfo.hpp"


namespace boost {
namespace python {

class object;

}
}



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
        void ChangeOption(const std::string & key, const T & value)
        {
            options_.Change(key, value);
        }


        // For use from python
        boost::python::object GetTrait(const std::string & key) const;
        boost::python::object GetOption(const std::string & key) const;

        // For use from python classes derived from this
        ModuleStore & MStore(void);
        


    protected:
        template<typename T>
        void ChangeTrait(const std::string & key, const T & value)
        {
            traits_.Change(key, value);
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
