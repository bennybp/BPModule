/*! \file
 *
 * \brief Storage of options for a module (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include <boost/python/dict.hpp>
#include "bpmodule/python_helper/Call.hpp"


#include "bpmodule/options/OptionMap.hpp"
#include "bpmodule/output/Output.hpp"


using namespace bpmodule::python_helper;
using namespace bpmodule::exception;
using namespace bpmodule::output;


namespace bpmodule {
namespace options {



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
    if(this != &rhs)
    {
        expert_ = rhs.expert_;
        wholevalid_ = rhs.wholevalid_;

        opmap_.clear();
        for(const auto & it : rhs.opmap_)
            opmap_.emplace(it.first, it.second->Clone());
    }
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





/*! \brief Wrap a python function that validates the entire OptionMap
 *
 * Calls python callable function \p func with \p val as the only argument
 *
 * \throw bpmodule::exception::PythonCallException if there is a problem calling
 *        the python function or if the return type can't be converted
 */
static WholeOptionMapIssues WholeOptValidateWrapper(const boost::python::object & func, const OptionMap & val)
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
OptionMap::OptionMap(const std::string & modulekey, const boost::python::dict & opt, const boost::python::object & wholevalidfunc)
    : modulekey_(modulekey), expert_(false), lockvalid_(false)
{
    boost::python::list keys = opt.keys();

    // shouldn't throw, should it?
    int keylen = boost::python::extract<int>(keys.attr("__len__")());

    for(int i = 0; i < keylen; i++)
    {
        if(DeterminePyType(keys[i]) != python_helper::PythonType::String)
            throw OptionException("Key in OptionMap dictionary is not a string", "(unknown)",
                                  "element", std::to_string(i),
                                  "pytype", GetPyClass(keys[i]));


        std::string key = ConvertToCpp<std::string>(keys[i]);

        // convert to lowercase
        output::ToLower(key);


        // should this ever happen? Keys should be unique in a
        // python dict
        if(opmap_.count(key))
            throw OptionException("Duplicate key on construction", key,
                                   "element", std::to_string(i));

        // this will throw needed exceptions
        opmap_.emplace(key, detail::OptionHolderFactory(key, opt[key]));
    }

    // add whole validator (if it exists)
    if(DeterminePyType(wholevalidfunc) != python_helper::PythonType::None)
    {
        // Don't forget that the method is part of a class
        // so 1 argument is "self"
        if(!python_helper::HasCallableAttr(wholevalidfunc, "Validate", 2))
            throw OptionException("Whole options validator does not have a callable Validate() method taking one argument", "(none)",
                                  "pytype", GetPyClass(wholevalidfunc));

        wholevalid_ = std::bind(WholeOptValidateWrapper, wholevalidfunc, std::placeholders::_1);
    }

}



void OptionMap::ChangePy(const std::string & key, const boost::python::object & obj)
{
    detail::OptionBase * ptr = GetOrThrow_(key);
    ptr->ChangePy(obj);

    if(lockvalid_)
        Validate();
}



void OptionMap::ChangePyDict(const boost::python::dict & opt)
{
    using std::swap;

    boost::python::list keys = opt.keys();
    int keylen = boost::python::extract<int>(keys.attr("__len__")());

    // for strong exception guarantee:
    // copy the current object, modify that, then swap
    OptionMap tmp(*this);

    // unlock for the moment. Some intermediate states may not be valid
    tmp.LockValid(false);

    for(int i = 0; i < keylen; i++)
    {
        if(DeterminePyType(keys[i]) != python_helper::PythonType::String)
            throw OptionException("Key in OptionMap dictionary is not a string", "(unknown)",
                                  "element", std::to_string(i),
                                  "pytype", GetPyClass(keys[i]));


        std::string key = ConvertToCpp<std::string>(keys[i]);

        // convert to lowercase
        output::ToLower(key);


        if(!tmp.HasKey(key))
            throw OptionException("Python dictionary has a key that I do not", key, "element", std::to_string(i));

        // may throw
        tmp.ChangePy(key, opt[key]);
    }

    // set the validity lock to whatever it is in this OptionMap
    // (also, will validate if needed)
    tmp.LockValid(lockvalid_);

    swap(*this, tmp);
}



boost::python::object OptionMap::GetPy(const std::string & key) const
{
    return GetOrThrow_(key)->GetPy();
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







} // close namespace options
} // close namespace bpmodule

