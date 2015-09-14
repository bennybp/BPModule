/*! \file
 *
 * \brief Conversion between python and C++
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_CONVERT_HPP_
#define _GUARD_CONVERT_HPP_

#include <boost/python.hpp>

#include "bpmodule/exception/PythonConvertException.hpp"


using bpmodule::exception::PythonConvertException;



namespace bpmodule {
namespace python_helper {


/*! \brief Convert a boost::python::object to a C++ type
 *
 * This function will check first, and throw if the check fails.
 *
 * \throw bpmodule::exception::PythonConvertException if the
 *        data could not be converted
 *
 * \tparam T The type to convert to
 *
 * \param [in] obj The object to convert
 * \return Converted data as type T
 */
template<typename T>
T ConvertToCpp(const boost::python::object & obj)
{
    boost::python::extract<T> conv(obj);
    if(!conv.check())
    {
        std::string fromtype = boost::python::extract<std::string>(obj.attr("__class__").attr("__name__"));
        throw PythonConvertException("Cannot convert from python to C++", fromtype, typeid(T).name(), "Check failed"); 
    }

    try {
        return conv();
    }
    catch(...)
    {
        std::string fromtype = boost::python::extract<std::string>(obj.attr("__class__").attr("__name__"));
        throw PythonConvertException("Cannot convert from python to C++", fromtype, typeid(T).name(), "Conversion failed"); 
    }
}



/*! \brief Convert a C++ object to a boost::python::object
 *
 * \throw bpmodule::exception::PythonConvertException if the
 *        data could not be converted
 *
 * \tparam T The type to convert from
 *
 * \param [in] obj The object to convert
 * \return Converted data as a boost::python::object
 */
template<typename T>
boost::python::object ConvertToPy(const T & obj)
{
    try {
       return boost::python::object(obj); 
    }
    catch(...)
    {
        throw PythonConvertException("Cannot convert from C++ to python", typeid(T).name(), "boost::python::object", "Conversion failed"); 
    }
}





/*! \brief Convert a boost::python::list to a C++ std::vector
 *
 * \throw bpmodule::exception::PythonConvertException if the
 *        data could not be converted
 *
 * \tparam T The type to convert to
 *
 * \param [in] list The python list to convert
 * \return Converted data as a C++ std::vector
 */
template<typename T>
std::vector<T> ConvertListToVec(const boost::python::list & list)
{
    int length = boost::python::extract<int>(list.attr("__len__")());
    std::vector<T> ret;
    ret.reserve(length);

    for (int i = 0; i < length; i++)
    {
        try {
            ret.push_back(ConvertToCpp<T>(list[i]));
        }
        catch(bpmodule::exception::PythonConvertException & ex)
        {
            ex.AppendInfo({{ "element", std::to_string(i) }});
            throw;
        }
    }

    return ret;
}



/*! \brief Convert a C++ vector to a boost::python::object
 *
 * \throw bpmodule::exception::PythonConvertException if the
 *        data could not be converted
 *
 * \tparam T The type to convert from
 *
 * \param [in] v The vector to convert
 * \return Converted data as a boost::python::list
 */
template<typename T>
boost::python::list ConvertVecToList(const std::vector<T> & v)
{
    boost::python::list result;

    for(size_t i = 0; i < v.size(); ++i)
    {
        try {
            result.append(ConvertToPy(v[i]));
        }
        catch(bpmodule::exception::PythonConvertException & ex)
        {
            ex.AppendInfo({ { "element", std::to_string(i)} });
            throw;
        }
    }

    return result;
}



} // close namespace python_helper
} // close namespace bpmodule

#endif
