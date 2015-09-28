/*! \file
 *
 * \brief The base class for all modules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/modulestore/ModuleStore.hpp"
#include "bpmodule/output/Output.hpp"

using bpmodule::modulestore::ModuleStore;
using bpmodule::modulestore::ModuleInfo;
using bpmodule::exception::GeneralException;


namespace bpmodule {
namespace modulebase {


ModuleBase::ModuleBase(unsigned long id,
                       ModuleStore & mstore,
                       ModuleInfo & minfo)
    : id_(id), minfo_(minfo),
      mstore_(mstore)
{
    output::Debug("Constructed module [%1%] : %2% v%3%\n", ID(), Name(), Version());
}



ModuleBase::~ModuleBase()
{
    output::Debug("Destructed module [%1%] : %2% v%3%\n", ID(), Name(), Version());
}


void ModuleBase::ThrowException(const std::string & exwhat,
                                const GeneralException::ExceptionInfo & exinfo) const
{
    GeneralException::ExceptionInfo exinfo2{
                                              { "Module Key", Key() },
                                              { "Module Name", Name() }
                                            };
    exinfo2.insert(exinfo2.end(), exinfo.begin(), exinfo.end());
    throw GeneralException(exwhat, exinfo2);
}



/////////////////////////////
// Basic Info
/////////////////////////////
unsigned long ModuleBase::ID(void) const noexcept
{
    return id_;
}



const std::string & ModuleBase::Key(void) const noexcept
{
    return minfo_.key;
}



const std::string & ModuleBase::Name(void) const noexcept
{
    return minfo_.name;
}



const std::string & ModuleBase::Version(void) const noexcept
{
    return minfo_.version;
}



ModuleStore & ModuleBase::MStore(void) noexcept
{
    return mstore_;
}


void ModuleBase::Print(void) const
{
    minfo_.Print();
}


datastore::OptionMap & ModuleBase::Options(void) noexcept
{
    return minfo_.options;
}

const datastore::OptionMap & ModuleBase::Options(void) const noexcept
{
    return minfo_.options;
}


} // close namespace modulebase
} // close namespace bpmodule
