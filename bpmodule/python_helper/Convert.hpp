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



/*! \brief Family of structs to convert python types to C++ types
 *
 * \tparam T Type to convert to
 */
template<typename T>
struct ToCppConverter
{
    /*! \brief Checks if a python object is convertable to type T
     *
     * \param [in] obj Object to check
     */ 
    static bool Check(const boost::python::object & obj)
    {
        boost::python::extract<T> conv(obj);
        return conv.check();
    }



    /*! \brief Converts a python object to type T
     *
     * \throwno May throw a boot::python exception
     *
     * \param [in] obj Object to check
     */ 
    static T Convert(const boost::python::object & obj)
    {
        boost::python::extract<T> conv(obj);
        return conv();
    }
};





/*! \brief Converts a python list to a std::vector
 *
 * \tparam T Type of the vector element to convert to
 */
template<typename T>
struct ToCppConverter<std::vector<T>>
{

    /*! \brief Checks if a python object is convertable to type std::vector<T>
     * 
     * Checks that the object is a boost::python::list and that
     * all elements are convertible to type T
     *
     * \param [in] obj Object to check
     */ 
    static bool Check(const boost::python::object & obj)
    {
        // Check if all elements are convertible
        boost::python::extract<boost::python::list> lconv(obj);
        if(!lconv.check())
            return false;

        boost::python::list lst = lconv();

        // ok if there are no elements
        int length = boost::python::extract<int>(lst.attr("__len__")());
        if(length == 0)
            return true;


        for (int i = 0; i < length; i++)
        {
            if(!ToCppConverter<T>::Check(lst[i]))
                return false;
        }

        // if you are here, everything is ok
        return true;
    }



    /*! \brief Converts a python object to type std::vector<T>
     *
     * \throwno May throw a boot::python exception
     *
     * \param [in] obj Object to check
     */ 
    static std::vector<T> Convert(const boost::python::object & obj)
    {
        std::vector<T> r;
 
        boost::python::list lst = boost::python::extract<boost::python::list>(obj);
        int length = boost::python::extract<int>(lst.attr("__len__")());
        if(length == 0)
            return r;

        r.reserve(length);

        for (int i = 0; i < length; i++)
        {
            try {
                r.push_back(ToCppConverter<T>::Convert(lst[i]));
            }
            catch(bpmodule::exception::PythonConvertException & ex)
            {
                ex.AppendInfo({{ "element", std::to_string(i) }});
                throw;
            }
        }

        return r;

    }
};







/*! \brief Convert a boost::python::object to a C++ type
 *
 * This wraps the ToCppConverter structures.
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
    if(!ToCppConverter<T>::Check(obj))
    {
        std::string fromtype = boost::python::extract<std::string>(obj.attr("__class__").attr("__name__"));
        throw PythonConvertException("Cannot convert from python to C++", fromtype, typeid(T).name(), "Check failed"); 
    }

    try {
        return ToCppConverter<T>::Convert(obj);
    }
    catch(...) //! \todo Doesn't seem to catch python exceptions?
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







/*! \brief Convert a C++ vector to a boost::python::list
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
boost::python::list ConvertToPy(const std::vector<T> & v)
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
