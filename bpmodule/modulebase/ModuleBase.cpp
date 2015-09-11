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
                       const ModuleInfo & minfo)
    : id_(id), key_(minfo.key), name_(minfo.name),
      version_(minfo.version), options_(minfo.options),
      mstore_(mstore)
{
    output::Debug("Constructed module [%1%] : %2% v%3%\n", id_, name_, version_);
}



ModuleBase::~ModuleBase()
{
    output::Debug("Destructed module [%1%] : %2% v%3%\n", id_, name_, version_);
}



void ModuleBase::ThrowException(const std::string & exwhat,
                                const GeneralException::ExceptionInfo & exinfo) const
{
    GeneralException::ExceptionInfo exinfo2{
                                              { "Module Key", key_ },
                                              { "Module Name", name_ }
                                            };
    exinfo2.insert(exinfo2.end(), exinfo.begin(), exinfo.end());
    throw GeneralException(exwhat, exinfo2);
}



unsigned long ModuleBase::ID(void) const noexcept
{
    return id_;
}



const std::string & ModuleBase::Key(void) const noexcept
{
    return key_;
}



const std::string & ModuleBase::Name(void) const noexcept
{
    return name_;
}



const std::string & ModuleBase::Version(void) const noexcept
{
    return version_;
}



ModuleStore & ModuleBase::MStore(void) noexcept
{
    return mstore_;
}



bool ModuleBase::HasOption(const std::string & key) const
{
    return options_.Has(key);
}



} // close namespace modulebase
} // close namespace bpmodule
