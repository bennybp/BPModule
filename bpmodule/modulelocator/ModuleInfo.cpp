/*! \file
 *
 * \brief Structure storing information about a module (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/python_helper/Convert.hpp"

#include "bpmodule/modulelocator/ModuleInfo.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/exception/ModuleLocatorException.hpp"


using bpmodule::datastore::OptionMap;
using bpmodule::python_helper::ConvertToCpp2;
using bpmodule::exception::GeneralException;
using bpmodule::exception::ModuleLocatorException;

namespace bpmodule {
namespace modulelocator {


//! \todo Becuase pybind11 doesn't have a has_key or similar
static bool DictHasKey_(pybind11::dict dictionary, const std::string & key)
{
    for(auto it : dictionary)
        if(ConvertToCpp2<std::string>(it.first) == key)
            return true;
    return false;
}


/*! \brief Converts types for ModuleInfo
 *
 * Catches exceptions in the conversion and appends some information
 *
 * \throw bpmodule::exception::ModuleLocatorException
 *        if there is a conversion problem or a
 *        required key doesn't exist
 *
 * \tparam T Destination type
 *
 * \param [in] dictionary The dictionary containing the data
 * \param [in] key The key in the dictionary
 */
template<typename T>
static T DictConvertHelper(pybind11::dict dictionary, const char * key)
{
    if(!DictHasKey_(dictionary, key))
        throw ModuleLocatorException("Python dictionary is missing key",
                                   "location", "ModuleInfo",
                                   "dictkey", key,
                                   "totype", util::DemangleCppType<T>());


    try {
        // may throw PythonConvertException
        return ConvertToCpp2<T>(dictionary[key]);
    }
    catch(const GeneralException & ex)
    {
        throw ModuleLocatorException(ex, "dictkey", key, "location", "ModuleInfo");
    }
}



/*! \brief Converts python lists to C++ vectors for ModuleInfo
 *
 * Catches exceptions in the conversion and appends some information
 *
 * \throw bpmodule::exception::ModuleLocatorException
 *        if there is a conversion problem or a
 *        required key doesn't exist
 *
 * \tparam T Destination type
 *
 * \param [in] dictionary The dictionary containing the data
 * \param [in] key The key in the dictionary that contains a python list
 */
template<typename T>
static std::vector<T> DictConvertHelperVec(pybind11::dict dictionary, const char * key)
{
    // will make sure it can convert to list
    // and will append key to exception
    pybind11::list lst = DictConvertHelper<pybind11::list>(dictionary, key);


    // Actually convert the vector
    try {
        return ConvertToCpp2<std::vector<T>>(lst);
    }
    catch(const GeneralException & ex)
    {
        throw ModuleLocatorException(ex, "dictkey", key, "location", "ModuleInfo");
    }
}





ModuleInfo::ModuleInfo(pybind11::dict dictionary)
{
    try {
        key         = DictConvertHelper<std::string>(dictionary, "key");
        name        = DictConvertHelper<std::string>(dictionary, "name");
        type        = DictConvertHelper<std::string>(dictionary, "type");
        path        = DictConvertHelper<std::string>(dictionary, "path");
        version     = DictConvertHelper<std::string>(dictionary, "version");
        description = DictConvertHelper<std::string>(dictionary, "description");

        authors     = DictConvertHelperVec<std::string>(dictionary, "authors");
        refs        = DictConvertHelperVec<std::string>(dictionary, "refs");


        // 'None' is ok for whole options validator
        pybind11::object wholeoptvalid;
        if(DictHasKey_(dictionary, "wholeoptvalidator"))
            wholeoptvalid = dictionary["wholeoptvalidator"];

        options     = OptionMap(key, DictConvertHelper<pybind11::dict>(dictionary, "options"), wholeoptvalid);

        // soname is optional
        if(DictHasKey_(dictionary, "soname"))
            soname = DictConvertHelper<std::string>(dictionary, "soname");
    }
    catch(const GeneralException & ex)
    {
        throw ModuleLocatorException(ex, "modulename", name, "modulekey", key); // name and key may or may not be set
    }
}



void ModuleInfo::Print(void) const
{
    output::Output("\n");
    output::Output("  ++ Module: %1%\n", name);
    output::Output("         Version: %1%\n", version);
    output::Output("             Key: %1%\n", key);
    output::Output("            Type: %1%\n", type);
    output::Output("            Path: %1%\n", path);

    if(soname.size())
        output::Output("          SOName: %1%\n", soname);

    output::Output("     Description: %1%\n", description);

    output::Output("         Authors: %1%\n", (authors.size() ? authors[0] : ""));
    for(size_t i = 1; i < authors.size(); i++)
        output::Output("                  %1%\n", authors[i]);

    output::Output("      References: %1%\n", (refs.size() ? refs[0] : ""));
    for(size_t i = 1; i < refs.size(); i++)
        output::Output("                  %1%\n", refs[i]);

    output::Output("         Options: %1%\n", options.Size());
    options.Print();
}




} // close namespace modulelocator
} // close namespace bpmodule

