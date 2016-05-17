/*! \file
 *
 * \brief Storage of options for a module (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/python/Call.hpp"


#include "pulsar/datastore/OptionMap.hpp"
#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/util/bphash/types/string.hpp"
#include "pulsar/util/bphash/types/memory.hpp"
#include "pulsar/util/bphash/types/map.hpp"


using namespace pulsar::python;
using namespace pulsar::exception;
using namespace pulsar::output;


namespace pulsar{
namespace datastore {


////////////////////////////////////////////////
// OptionMapIssues Member functions
////////////////////////////////////////////////
bool OptionMapIssues::OK(void) const
{
    return (toplevel.size() == 0 && optissues.size() == 0);
}

void OptionMapIssues::Print(std::ostream & os) const
{
    if(toplevel.size() || optissues.size())
        Warning(os, "OptionMap has some issues\n");

    if(toplevel.size())
    {
        Warning(os, "    OptionMap top level issues:\n");
        for(const auto & it : toplevel)
            Warning(os, "        %?\n", it);
    }
    if(optissues.size())
    {
        Warning(os, "    Individual option issues:\n");
        for(const auto & it : optissues)
        {
            Warning(os, "        %?\n", it.first);
            for(const auto & it2 : it.second)
                Warning(os, "            %?\n", it2);
        }
    }
}

std::string OptionMapIssues::String(void) const
{
    std::stringstream ss;
    Print(ss);
    return ss.str();
}

////////////////////////////////////////////////
// Member functions
////////////////////////////////////////////////
OptionMap::OptionMap(void)
    : expert_(false)
{ }


OptionMap::OptionMap(const OptionMap & rhs)
    : expert_(rhs.expert_),
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

bool OptionMap::IsSet(const std::string & key) const
{
    return GetOrThrow_(key)->IsSet();
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

bool OptionMap::IsExpert(void) const noexcept
{
    return expert_;
}



KeySet OptionMap::AllMissingReq(void) const
{
    KeySet req;
    for(const auto & it : opmap_)
        if(!it.second->IsSetIfRequired())
            req.insert(it.first);

    return req;
}


const detail::OptionBase * OptionMap::GetOrThrow_(const std::string & key) const
{
    if(opmap_.count(key))
        return opmap_.at(key).get();
    else
        throw OptionException("Option key not found", "optionkey", key);
}

detail::OptionBase * OptionMap::GetOrThrow_(const std::string & key)
{
    if(opmap_.count(key))
        return opmap_.at(key).get();
    else
        throw OptionException("Option key not found", "optionkey", key);
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
    return !(GetIssues().OK());
}


bool OptionMap::Compare(const OptionMap & rhs) const
{
    if(Size() != rhs.Size())
        return false;

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

bool OptionMap::operator==(const OptionMap & rhs) const
{
    return Compare(rhs);
}


void OptionMap::AddOption(std::string key, OptionType opttype, bool required,
                          const pybind11::object & validator, std::string help,
                          const pybind11::object & def)
{
    if(HasKey(key))
        throw OptionException("Attempting to add duplicate option key",
                              "optionkey", key);

    detail::OptionBasePtr oph = detail::CreateOptionHolder(key, opttype, required,
                                                           validator, help, def);

    opmap_.emplace(std::move(key), std::move(oph));
}

util::Hash OptionMap::MyHash(void) const
{
    return util::MakeHash(*this);
}

util::Hash OptionMap::HashValues(const std::set<std::string> & keys) const
{
    util::Hasher h;

    for(const auto & it : keys)
    {
        const detail::OptionBase * op = GetOrThrow_(it);
        op->HashValue(h);
    }

    return h.Finalize();
}

util::Hash OptionMap::HashAllValues(void) const
{
    util::Hasher h;

    for(const auto & it : opmap_)
        it.second->HashValue(h);

    return h.Finalize();
}


void OptionMap::hash(util::Hasher & h) const
{
    h(expert_, opmap_);
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
    detail::OptionBase * ptr = GetOrThrow_(key);
    ptr->ChangePy(obj);
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
        std::string s8(8, '-');
        Output(os, "    %-20?   %-20?   %-8?   %-20?   %-20?   %-8?   %?\n", "Option", "Type", "IsSet", "Value", "Default", "Required", "Description");
        Output(os, "    %-20?   %-20?   %-8?   %-20?   %-20?   %-8?   %?\n", s20, s20, s8, s20, s20, s8, s20);

        for(const auto & it : opmap_)
            it.second->Print(os);
    }
    Output(os, "\n");
}







} // close namespace datastore
} // close namespace pulsar

