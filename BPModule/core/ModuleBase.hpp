#ifndef MODULEBASE_H
#define MODULEBASE_H

#include <string>
#include <functional>

#include "BPModule/core/ModuleInfo.hpp"
#include "BPModule/core/CalcData.hpp"

namespace bpy = boost::python;


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
        T GetOption(const std::string & key) const
        {
            try {
                return options_.Get<T>(key);
            }
            catch(BPModuleException & ex)
            {
                // rethrow with module info
                ThrowException(ex.what(), ex.GetInfo());
                return T(); // to make compilers happy
            }
        }


        bool HasOption(const std::string & key) const;

        
    protected:
        void ThrowException(const std::string & exwhat,
                            const BPModuleException::ExceptionInfo & exinfo = {}) const;

        ModuleStore & MStore(void);

        void ThrowException(const std::string & exwhat,
                            const bpy::list & exinfo);


    private:
        unsigned long id_;
        std::string key_;
        std::string name_;
        std::string version_;
        OptionMap options_;

        ModuleStore & mstore_;
};


} // close namespace bpmodule

#endif
