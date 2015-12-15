/*! \file
 *
 * \brief Storage of options for a module (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/python_helper/Call.hpp"


#include "bpmodule/datastore/OptionMap.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/exception/OptionException.hpp"


using namespace bpmodule::python_helper;
using namespace bpmodule::exception;
using namespace bpmodule::output;


namespace bpmodule {
namespace datastore {



////////////////////////////////////////////////
// Member functions
////////////////////////////////////////////////

OptionMap::OptionMap(const OptionMap & rhs)
    : modulekey_(rhs.modulekey_),
      expert_(rhs.expert_),
      lockvalid_(rhs.lockvalid_),
      wholevalid_(rhs.wholevalid_)
{
    for(const auto & it : rhs.opmap_)
        opmap_.emplace(it.first, it.second->Clone());
}


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
        if(!it.second->IsSetIfRequired())
            req.push_back(it.first);

    return req;
}


detail::OptionBase * OptionMap::GetOrThrow_(const std::string & key)
{
    if(opmap_.count(key))
        return opmap_.at(key).get();
    else
        throw OptionException("Key not found", key);
}


const detail::OptionBase * OptionMap::GetOrThrow_(const std::string & key) const
{
    if(opmap_.count(key))
        return opmap_.at(key).get();
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
        detail::OptionIssues oi = it.second->GetIssues();
        if(oi.size())
            omi.optissues.emplace(it.first, oi);
    }

    return omi;
}


bool OptionMap::HasIssues(void) const
{
    OptionMapIssues omi = GetIssues();
    return (omi.toplevel.size() == 0 && omi.optissues.size() == 0);
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
        if(!(rhs.opmap_.at(it.first)->Compare(*it.second)))
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
            if(opmap_.at(it)->Compare(*rhs.opmap_.at(it)) == false)
                return false;

        // if neither have it that's ok?
    }

    return true;
}



/*! \brief Wrap a python function that validates the entire OptionMap
 *
 * Calls python callable function \p func with \p val as the only argument
 *
 * \throw bpmodule::exception::PythonCallException if there is a problem calling
 *        the python function or if the return type can't be converted
 */
static WholeOptionMapIssues WholeOptValidateWrapper(pybind11::object func, const OptionMap & val)
{
    try {
        return CallPyFunc<std::vector<std::string>>(func, val);
    }
    catch(PythonCallException & ex)
    {
        ex.AppendInfo("when", "while Calling validator function for an option map", "modulekey", val.ModuleKey());
        throw;
    }
}





//////////////////////////////
// Python functions
//////////////////////////////
OptionMap::OptionMap(const std::string & modulekey, pybind11::dict opt, pybind11::object wholevalidfunc)
    : modulekey_(modulekey), expert_(false), lockvalid_(false)
{
    for(auto it : opt)
    {
        if(DeterminePyType(it.first) != python_helper::PythonType::String)
            throw OptionException("Key in OptionMap dictionary is not a string", "(unknown)",
                                  "pytype", GetPyClass(it.first));


        std::string key = ConvertToCpp<std::string>(it.first);

        // convert to lowercase
        util::ToLower(key);


        // should this ever happen? Keys should be unique in a
        // python dict
        if(opmap_.count(key))
            throw OptionException("Duplicate key on construction", key);

        // this will throw needed exceptions
        opmap_.emplace(key, detail::OptionHolderFactory(key, it.second));
    }

    // add whole validator (if it exists)
    if(DeterminePyType(wholevalidfunc) != python_helper::PythonType::None)
    {
        // Don't forget that the method is part of a class
        // so 1 argument is "self"
        //! \todo Reimplement the version of HasCallableAttr with number of arguments
        if(!python_helper::HasCallableAttr(wholevalidfunc, "Validate"))
            throw OptionException("Whole options validator does not have a callable Validate() method taking one argument", "(none)",
                                  "pytype", GetPyClass(wholevalidfunc));

        wholevalid_ = std::bind(WholeOptValidateWrapper, wholevalidfunc, std::placeholders::_1);
    }

}



void OptionMap::ChangePy(const std::string & key, pybind11::object obj)
{
    detail::OptionBase * ptr = GetOrThrow_(key);
    ptr->ChangePy(obj);

    if(lockvalid_)
        Validate();
}



void OptionMap::ChangePyDict(pybind11::dict opt)
{
    using std::swap;

    // for strong exception guarantee:
    // copy the current object, modify that, then swap
    OptionMap tmp(*this);

    // unlock for the moment. Some intermediate states may not be valid
    tmp.LockValid(false);

    for(auto it : opt)
    {
        if(DeterminePyType(it.first) != python_helper::PythonType::String)
            throw OptionException("Key in OptionMap dictionary is not a string", "(unknown)",
                                  "pytype", GetPyClass(it.first));


        std::string key = ConvertToCpp<std::string>(it.first);

        // convert to lowercase
        util::ToLower(key);


        if(!tmp.HasKey(key))
            throw OptionException("Python dictionary has a key that I do not", key);

        // may throw
        tmp.ChangePy(key, it.second);
    }

    // set the validity lock to whatever it is in this OptionMap
    // (also, will validate if needed)
    tmp.LockValid(lockvalid_);

    swap(*this, tmp);
}



pybind11::object OptionMap::GetPy(const std::string & key) const
{
    return GetOrThrow_(key)->GetPy();
}


bool OptionMap::CompareSelectPy(const OptionMap & rhs, pybind11::list selection) const
{
    //! \todo exceptions
    return CompareSelect(rhs, ConvertToCpp<std::vector<std::string>>(selection));
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
            it.second->Print();
    }
    Output("\n");
}







} // close namespace datastore
} // close namespace bpmodule

