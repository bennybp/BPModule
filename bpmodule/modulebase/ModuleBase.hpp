#ifndef MODULEBASE_H
#define MODULEBASE_H

#include "bpmodule/datastore/CalcData.hpp"
#include "bpmodule/datastore/OptionMap.hpp"

// forward declarations
namespace bpmodule {
namespace modulestore {
class ModuleStore;
class ModuleInfo;
}
}
// end forward declarations


namespace bpmodule {
namespace modulebase {


class ModuleBase
{
    public:
        ModuleBase(unsigned long id,
                   modulestore::ModuleStore & mstore,
                   const modulestore::ModuleInfo & minfo);

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
            catch(exception::GeneralException & ex)
            {
                // rethrow with module info
                ThrowException(ex.what(), ex.GetInfo());
                return T(); // to make compilers happy
            }
        }


        bool HasOption(const std::string & key) const;

        
    protected:
        void ThrowException(const std::string & exwhat,
                            const exception::GeneralException::ExceptionInfo & exinfo = {}) const;

        modulestore::ModuleStore & MStore(void);

        void ThrowException(const std::string & exwhat,
                            const boost::python::list & exinfo);


    private:
        unsigned long id_;
        std::string key_;
        std::string name_;
        std::string version_;
        datastore::OptionMap options_;

        modulestore::ModuleStore & mstore_;
};


} // close namespace modulebase
} // close namespace bpmodule

#endif
