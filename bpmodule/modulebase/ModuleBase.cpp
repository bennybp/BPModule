/*! \file
 *
 * \brief The base class for all modules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/modulelocator/ModuleLocator.hpp"
#include "bpmodule/output/Output.hpp"

using bpmodule::modulelocator::ModuleLocator;
using bpmodule::modulelocator::ModuleInfo;
using bpmodule::exception::GeneralException;


namespace bpmodule {
namespace modulebase {


ModuleBase::ModuleBase(unsigned long id,
                       ModuleLocator & mlocator,
                       ModuleInfo & minfo)
    : id_(id), minfo_(minfo),
      mlocator_(mlocator)
{
    output::Debug("Constructed module [%1%] : %2% v%3%\n", ID(), Name(), Version());
}



ModuleBase::~ModuleBase()
{
    output::Debug("Destructed module [%1%] : %2% v%3%\n", ID(), Name(), Version());
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



ModuleLocator & ModuleBase::MLocator(void) noexcept
{
    return mlocator_;
}


void ModuleBase::Print(void) const
{
    minfo_.Print();
}


options::OptionMap & ModuleBase::Options(void) noexcept
{
    return minfo_.options;
}

const options::OptionMap & ModuleBase::Options(void) const noexcept
{
    return minfo_.options;
}


} // close namespace modulebase
} // close namespace bpmodule
