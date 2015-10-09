/*! \file
 *
 * \brief Storage of options for a module (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include <boost/python.hpp>

#include "bpmodule/options/OptionMap.hpp"
#include "bpmodule/python_helper/Convert.hpp"
#include "bpmodule/output/Output.hpp"


using bpmodule::python_helper::ConvertToCpp;
using bpmodule::python_helper::DeterminePyType;
using bpmodule::python_helper::GetPyClass;
using bpmodule::exception::OptionException;
using bpmodule::output::Output;


namespace bpmodule {
namespace options {



////////////////////////////////////////////////
// Member functions
////////////////////////////////////////////////

OptionMap::OptionMap(const OptionMap & rhs)
{
    for(const auto & it : rhs.opmap_)
        opmap_.emplace(it.first, detail::OptionBasePtr(it.second->Clone()));
}


OptionMap & OptionMap::operator=(const OptionMap & rhs)
{
    if(this != &rhs)
    {
        opmap_.clear();
        for(const auto & it : rhs.opmap_)
            opmap_.emplace(it.first, detail::OptionBasePtr(it.second->Clone()));
    }
    return *this;
}


bool OptionMap::Has(const std::string & key) const
{
    std::string lkey = LowerString_(key);
    if(opmap_.count(lkey) == 0)
        return false;
    return opmap_.at(lkey)->HasValue();
}


bool OptionMap::HasKey(const std::string & key) const
{
    std::string lkey = LowerString_(key);
    return opmap_.count(lkey);
}

size_t OptionMap::Size(void) const noexcept
{
    return opmap_.size();
}


bool OptionMap::IsDefault(const std::string & key) const
{
    std::string lkey = LowerString_(key);
    return GetOrThrow_(lkey)->IsDefault();
}

void OptionMap::ResetToDefault(const std::string & key)
{
    std::string lkey = LowerString_(key);
    GetOrThrow_(lkey)->ResetToDefault();
}


bool OptionMap::IsValid(void) const noexcept
{
    for(const auto & it : opmap_)
        if(!it.second->IsValid())
            return false;
    return true;
}


detail::OptionBasePtr & OptionMap::GetOrThrow_(const std::string & key)
{
    if(opmap_.count(key))
        return opmap_.at(key);
    else
        throw OptionException("Key not found", key);
}


const detail::OptionBasePtr & OptionMap::GetOrThrow_(const std::string & key) const
{
    if(opmap_.count(key))
        return opmap_.at(key);
    else
        throw OptionException("Key not found", key);
}


std::string OptionMap::LowerString_(const std::string & str)
{
    //! \todo assume ASCII
    std::string ret(str);
    std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
    return ret;
}




//////////////////////////////
// Python functions
//////////////////////////////
OptionMap::OptionMap(const boost::python::dict & opt)
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

        std::string key = LowerString_(ConvertToCpp<std::string>(keys[i]));

        // should this ever happen? Keys should be unique in a
        //  python dict
        if(opmap_.count(key))
            throw OptionException("Duplicate key on construction", key,
                                   "element", std::to_string(i));

        // this will throw needed exceptions
        opmap_.emplace(key, detail::OptionHolderFactory(key, opt[key]));
    }
}



void OptionMap::ChangePy(const std::string & key, const boost::python::object & obj)
{
    std::string lkey = LowerString_(key);
    detail::OptionBasePtr & ptr = GetOrThrow_(lkey);

    ptr->ChangePy(obj);
}



void OptionMap::ChangePyDict(const boost::python::dict & opt)
{
    using std::swap;

    boost::python::list keys = opt.keys();
    int keylen = boost::python::extract<int>(keys.attr("__len__")());

    // for strong exception guarantee:
    // copy the current object, modify that, then swap

    OptionMap tmp(*this);

    for(int i = 0; i < keylen; i++)
    {
        if(DeterminePyType(keys[i]) != python_helper::PythonType::String)
            throw OptionException("Cannot convert python dictionary index to string", "(unknown)", 
                                  "element", std::to_string(i),
                                  "pytype", GetPyClass(keys[i]));

        std::string key = LowerString_(ConvertToCpp<std::string>(keys[i]));

        if(!tmp.HasKey(key))
            throw OptionException("Python dictionary has a key that I do not", key, "element", std::to_string(i));

        tmp.ChangePy(key, opt[key]);
    }

    swap(*this, tmp);
}



boost::python::object OptionMap::GetPy(const std::string & key) const
{
    std::string lkey = LowerString_(key);
    return GetOrThrow_(lkey)->GetPy();
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

