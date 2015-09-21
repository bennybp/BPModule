/*! \file
 *
 * \brief Structure storing information about a module (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/modulestore/ModuleInfo.hpp"
#include "bpmodule/python_helper/Convert.hpp"

using bpmodule::datastore::OptionMap;
using bpmodule::python_helper::ConvertToCpp;

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
        throw bpmodule::exception::PythonConvertException("Python dictionary missing key", "ModuleInfo", key, "(missing)", typeid(T).name());


    try {
        return ConvertToCpp<T>(dictionary[key]);
    }
    catch(bpmodule::exception::PythonConvertException & ex)
    {
        ex.AppendInfo({ {"key", key}, {"location", "ModuleInfo"} });
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
    catch(bpmodule::exception::PythonConvertException & ex)
    {
        ex.AppendInfo({ {"key", key}, {"location", "ModuleInfo"} });
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
        catch(bpmodule::exception::PythonConvertException & ex)
        {
            ex.AppendInfo({ {"key", "options"} });
            throw;
        }

        // soname is optional
        if(dictionary.has_key("soname"))
            soname = DictConvertHelper<std::string>(dictionary, "soname");
    }
    catch(bpmodule::exception::PythonConvertException & ex)
    {
        ex.AppendInfo({ {"module", name} }); // name may or may not be set
        throw;
    }
}


} // close namespace modulestore
} // close namespace bpmodule

