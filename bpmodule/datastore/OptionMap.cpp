/*! \file
 *
 * \brief Storage of options for a module (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/python/Call.hpp"


#include "bpmodule/datastore/OptionMap.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/exception/OptionException.hpp"


using namespace bpmodule::python;
using namespace bpmodule::exception;
using namespace bpmodule::output;


namespace bpmodule {
namespace datastore {



////////////////////////////////////////////////
// Member functions
////////////////////////////////////////////////


OptionMap & OptionMap::operator=(const OptionMap & rhs)
{
    using std::swap;

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
    return opmap_.at(key).HasValue();
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
    return GetOrThrow_(key).IsDefault();
}



void OptionMap::ResetToDefault(const std::string & key)
{
    GetOrThrow_(key).ResetToDefault();
}



bool OptionMap::AllReqSet(void) const noexcept
{
    for(const auto & it : opmap_)
        if(!it.second.IsSetIfRequired())
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
        output::Warning("OptionMap has some issues\n");

    if(omi.toplevel.size())
    {
        throwme = true;
        output::Warning("    OptionMap top level issues:\n");
        for(const auto & it : omi.toplevel)
            output::Warning("        %1%", it);
    }
    if(omi.optissues.size())
    {
        output::Warning("    Individual option issues:\n");
        for(const auto & it : omi.optissues)
        {
            output::Warning("        %1%\n", it.first);
            for(const auto & it2 : it.second)
                output::Warning("            %1%\n", it2);
        }
        throwme = true;
    }

    if(throwme)
    {
        if(expert_)
            output::Warning("Expert mode is set for the OptionMap. You're on you're own\n");
        else
            throw exception::GeneralException("OptionMap is in an invalid state. See above for errors");
    }
}


std::vector<std::string> OptionMap::AllMissingReq(void) const
{
    std::vector<std::string> req;
    for(const auto & it : opmap_)
        if(!it.second.IsSetIfRequired())
            req.push_back(it.first);

    return req;
}


const OptionHolder & OptionMap::GetOrThrow_(const std::string & key) const
{
    if(opmap_.count(key))
        return opmap_.at(key);
    else
        throw OptionException("Key not found", key);
}

OptionHolder & OptionMap::GetOrThrow_(const std::string & key)
{
    if(opmap_.count(key))
        return opmap_.at(key);
    else
        throw OptionException("Key not found", key);
}


OptionMapIssues OptionMap::GetIssues(void) const
{
    OptionMapIssues omi;

    if(wholevalid_)
        omi.toplevel = wholevalid_(*this);

    for(const auto & it : opmap_)
    {
        OptionIssues oi = it.second.GetIssues();
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
    //! \todo easier way?
    std::vector<std::string> keys1, keys2;
    for(const auto & it : opmap_)
        keys1.push_back(it.first);
    for(const auto & it : rhs.opmap_)
        keys2.push_back(it.first);
    if(keys1 != keys2)
        return false;

    for(const auto & it : opmap_)
        if(!(rhs.opmap_.at(it.first).Compare(it.second)))
            return false;

    return true;
}


bool OptionMap::CompareSelect(const OptionMap & rhs, const std::vector<std::string> & selection) const
{
    //! \todo easier way?
    //! \todo Too strict or not strict enough? What if one doesn't have it but the other doesn't have a value?
    for(const auto & it : selection)
    {
        // if one has it but not the other
        if(HasKey(it) != rhs.HasKey(it))
            return false;

        // if they both have it
        if(HasKey(it) && rhs.HasKey(it))
            if(opmap_.at(it).Compare(rhs.opmap_.at(it)) == false)
                return false;

        // if neither have it that's ok?
    }

    return true;
}


void OptionMap::AddOption(const std::string & key, const pybind11::object & def,
                          const pybind11::object & validator,
                          bool required, const std::string & pytype, const std::string & help)
{
    //! \todo insert sanity check for pytype / actual type mapping
    if(HasKey(key))
        throw OptionException("Attempting to add duplicate key", key, "module", modulekey_);

    opmap_.emplace(key, OptionHolder(key, def, validator, required, pytype, help));
}




//////////////////////////////////////
// Printing of options
/////////////////////////////////////
void OptionMap::Print(void) const
{
    size_t nopt = Size();
    if(nopt > 0)
    {
        Output("\n");
        std::string s20(20, '-');
        std::string s10(10, '-');
        Output("          %|1$-20|      %|2$-20|      %|3$-20|      %|4$-20|     %|5$-10|       %6%\n", "Option", "Type", "Value", "Default", "Required", "Description");
        Output("          %|1$-20|      %|2$-20|      %|3$-20|      %|4$-20|     %|5$-10|       %6%\n", s20, s20, s20, s20, s10, s20);

        for(const auto & it : opmap_)
            it.second.Print();
    }
    Output("\n");
}







} // close namespace datastore
} // close namespace bpmodule

