/*! \file
 *
 * \brief Storage of options for a module (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/python/Call.hpp"


#include "bpmodule/datastore/OptionMap.hpp"
#include "bpmodule/output/GlobalOutput.hpp"
#include "bpmodule/exception/Exceptions.hpp"


using namespace bpmodule::python;
using namespace bpmodule::exception;
using namespace bpmodule::output;


namespace bpmodule {
namespace datastore {



////////////////////////////////////////////////
// Member functions
////////////////////////////////////////////////

OptionMap::OptionMap(const std::string & modulekey)
    : modulekey_(modulekey)
{ }


OptionMap::OptionMap(const OptionMap & rhs)
    : modulekey_(rhs.modulekey_),
      expert_(rhs.expert_),
      lockvalid_(rhs.lockvalid_),
      wholevalid_(rhs.wholevalid_)
{
    for(const auto & it : rhs.opmap_)
        opmap_.emplace(it.first, std::move(it.second->Clone()));
}

OptionMap & OptionMap::operator=(const OptionMap & rhs)
{
    using std::swap;
    if(this == &rhs)
        return *this;

    // copy and swap it
    OptionMap copy(rhs);
    swap(*this, copy);
    return *this;
}



const std::string & OptionMap::ModuleKey(void) const noexcept
{
    return modulekey_;
}



bool OptionMap::Has(const std::string & key) const
{
    if(opmap_.count(key) == 0)
        return false;
    return opmap_.at(key)->HasValue();
}


bool OptionMap::HasKey(const std::string & key) const
{
    return opmap_.count(key);
}



size_t OptionMap::Size(void) const noexcept
{
    return opmap_.size();
}


bool OptionMap::IsDefault(const std::string & key) const
{
    return GetOrThrow_(key)->IsDefault();
}



void OptionMap::ResetToDefault(const std::string & key)
{
    GetOrThrow_(key)->ResetToDefault();
}



bool OptionMap::AllReqSet(void) const noexcept
{
    for(const auto & it : opmap_)
        if(!it.second->IsSetIfRequired())
            return false;
    return true;
}



void OptionMap::SetExpert(bool expert) noexcept
{
    expert_ = expert;
}



void OptionMap::LockValid(bool lockvalid)
{
    lockvalid_ = lockvalid;
}



void OptionMap::Validate(void) const
{
    OptionMapIssues omi = GetIssues();
    bool throwme = false;

    if(omi.toplevel.size() || omi.optissues.size())
        output::GlobalWarning("OptionMap has some issues\n");

    if(omi.toplevel.size())
    {
        throwme = true;
        output::GlobalWarning("    OptionMap top level issues:\n");
        for(const auto & it : omi.toplevel)
            output::GlobalWarning("        %?", it);
    }
    if(omi.optissues.size())
    {
        output::GlobalWarning("    Individual option issues:\n");
        for(const auto & it : omi.optissues)
        {
            output::GlobalWarning("        %?\n", it.first);
            for(const auto & it2 : it.second)
                output::GlobalWarning("            %?\n", it2);
        }
        throwme = true;
    }

    if(throwme)
    {
        if(expert_)
            output::GlobalWarning("Expert mode is set for the OptionMap. You're on you're own\n");
        else
            throw exception::OptionException("OptionMap is in an invalid state. See above for errors",
                                              "modulekey", modulekey_);
    }
}


KeySet OptionMap::AllMissingReq(void) const
{
    KeySet req;
    for(const auto & it : opmap_)
        if(!it.second->IsSetIfRequired())
            req.insert(it.first);

    return req;
}


const OptionBase * OptionMap::GetOrThrow_(const std::string & key) const
{
    if(opmap_.count(key))
        return opmap_.at(key).get();
    else
        throw OptionException("Option key not found", "optionkey", key, "modulekey", modulekey_);
}

OptionBase * OptionMap::GetOrThrow_(const std::string & key)
{
    if(opmap_.count(key))
        return opmap_.at(key).get();
    else
        throw OptionException("Option key not found", "optionkey", key, "modulekey", modulekey_);
}


OptionMapIssues OptionMap::GetIssues(void) const
{
    OptionMapIssues omi;

    if(wholevalid_)
        omi.toplevel = wholevalid_(*this);

    for(const auto & it : opmap_)
    {
        OptionIssues oi = it.second->GetIssues();
        if(oi.size())
            omi.optissues.emplace(it.first, oi);
    }

    return omi;
}


bool OptionMap::HasIssues(void) const
{
    OptionMapIssues omi = GetIssues();
    return (omi.toplevel.size() && omi.optissues.size());
}


bool OptionMap::Compare(const OptionMap & rhs) const
{
    KeySet keys1, keys2;
    for(const auto & it : opmap_)
        keys1.insert(it.first);
    for(const auto & it : rhs.opmap_)
        keys2.insert(it.first);
    if(keys1 != keys2)
        return false;

    for(const auto & it : opmap_)
        if(!(rhs.opmap_.at(it.first)->Compare(*(it.second))))
            return false;

    return true;
}


bool OptionMap::CompareSelect(const OptionMap & rhs, const KeySet & selection) const
{
    //! \todo Too strict or not strict enough? What if one doesn't have it but the other doesn't have a value?
    for(const auto & it : selection)
    {
        // if one has it but not the other
        if(HasKey(it) != rhs.HasKey(it))
            return false;

        // if they both have it
        if(HasKey(it) && rhs.HasKey(it))
            if(opmap_.at(it)->Compare(*(rhs.opmap_.at(it))) == false)
                return false;

        // if neither have it that's ok?
    }

    return true;
}


void OptionMap::AddOption(std::string key, OptionType opttype, bool required,
                          const pybind11::object & validator, std::string help,
                          const pybind11::object & def)
{
    if(HasKey(key))
        throw OptionException("Attempting to add duplicate option key",
                              "optionkey", key, "modulekey", modulekey_);

    std::unique_ptr<OptionBase> oph = CreateOptionHolder(key, opttype, required,
                                                       validator, help, def);

    opmap_.emplace(std::move(key), std::move(oph));
}



//////////////////////
// Python
//////////////////////
pybind11::object OptionMap::GetPy(const std::string & key) const
{
    return GetOrThrow_(key)->GetPy();
}


void OptionMap::ChangePy(const std::string & key, const pybind11::object & obj)
{
    OptionBase * ptr = GetOrThrow_(key);
    ptr->ChangePy(obj);

    if(lockvalid_)
        Validate();
}



//////////////////////////////////////
// Printing of options
/////////////////////////////////////
void OptionMap::Print(std::ostream & os) const
{
    size_t nopt = Size();
    if(nopt > 0)
    {
        Output(os, "\n");
        std::string s20(20, '-');
        std::string s10(10, '-');
        Output(os, "          %-20?      %-20?      %-20?      %-20?     %-10?       %?\n", "Option", "Type", "Value", "Default", "Required", "Description");
        Output(os, "          %-20?      %-20?      %-20?      %-20?     %-10?       %?\n", s20, s20, s20, s20, s10, s20);

        for(const auto & it : opmap_)
            it.second->Print(os);
    }
    Output(os, "\n");
}







} // close namespace datastore
} // close namespace bpmodule

