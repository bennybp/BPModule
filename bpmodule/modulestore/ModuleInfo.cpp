/*! \file
 *
 * \brief Structure storing information about a module (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/modulestore/ModuleInfo.hpp"
#include "bpmodule/python_helper/Convert.hpp"
#include "bpmodule/output/Output.hpp"


using bpmodule::datastore::OptionMap;
using bpmodule::python_helper::ConvertToCpp;
using bpmodule::exception::GeneralException;

namespace bpmodule {
namespace modulestore {


/*! \brief Converts types for ModuleInfo
 *
 * Catches exceptions in the conversion and appends some information
 *
 * \throw bpmodule::exception::PythonConvertException if there is a conversion problem or a
 *        required key doesn't exist
 *
 * \tparam T Destination type
 *
 * \param [in] dictionary The dictionary containing the data
 * \param [in] key The key in the dictionary
 */
template<typename T>
static T DictConvertHelper(const boost::python::dict & dictionary, const char * key)
{
    if(!dictionary.has_key(key))
        throw GeneralException("Python dictionary is missing key",
                               "location", "ModuleInfo",
                               "dictkey", key,
                               "totype", typeid(T).name());


    try {
        return ConvertToCpp<T>(dictionary[key]);
    }
    catch(GeneralException & ex)
    {
        ex.AppendInfo("dictkey", key, "location", "ModuleInfo");
        throw;
    }
}



/*! \brief Converts python lists to C++ vectors for ModuleInfo
 *
 * Catches exceptions in the conversion and appends some information
 *
 * \throw bpmodule::exception::PythonConvertException if there is a conversion problem or a
 *        required key doesn't exist
 *
 * \tparam T Destination type
 *
 * \param [in] dictionary The dictionary containing the data
 * \param [in] key The key in the dictionary
 */
template<typename T>
static std::vector<T> DictConvertHelperVec(const boost::python::dict & dictionary, const char * key)
{
    // will make sure it can convert to list
    // and will append key to exception
    boost::python::list lst = DictConvertHelper<boost::python::list>(dictionary, key);


    // Actually convert the vector
    try {
        return ConvertToCpp<std::vector<T>>(lst);
    }
    catch(GeneralException & ex)
    {
        ex.AppendInfo("dictkey", key, "location", "ModuleInfo");
        throw;
    }
}





ModuleInfo::ModuleInfo(const boost::python::dict & dictionary)
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

        try {
            options = OptionMap(DictConvertHelper<boost::python::dict>(dictionary, "options"));
        }
        catch(GeneralException & ex)
        {
            ex.AppendInfo("key", key);
            throw;
        }

        // soname is optional
        if(dictionary.has_key("soname"))
            soname = DictConvertHelper<std::string>(dictionary, "soname");
    }
    catch(GeneralException & ex)
    {
        ex.AppendInfo("modulename", name); // name may or may not be set
        throw;
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
    for(size_t i = 0; i < authors.size(); i++)
        output::Output("                  %1%\n", authors[i]);

    output::Output("      References: %1%\n", (refs.size() ? refs[0] : ""));      
    for(size_t i = 0; i < refs.size(); i++)
        output::Output("                  %1%\n", refs[i]);
   
    output::Output("         Options: %1%\n", options.Size());
    options.Print(); 
}




} // close namespace modulestore
} // close namespace bpmodule

