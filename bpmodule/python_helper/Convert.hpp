/*! \file
 *
 * \brief Conversion between python and C++ (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_CONVERT_HPP_
#define _GUARD_CONVERT_HPP_

#include <boost/python.hpp>

#include "bpmodule/exception/GeneralException.hpp"


namespace bpmodule {
namespace python_helper {


//! Types defined in python that are available in BPModule
enum class PythonType
{
    Bool,
    Int,
    Float,
    String,
    ListBool,
    ListInt,
    ListFloat,
    ListString,
    ListEmpty,
    ListHetero,
    ListUnknown,
    Tuple,
    Dict,
    None,
    Unknown
};




// In cpp file


/*! \brief Convert a string to a PythonType
 */ 
PythonType StrToPythonType(const std::string & str) noexcept;


/*! \brief Convert a PythonType to a string
 */ 
const char * PythonTypeToStr(PythonType pytype) noexcept;


/*! \brief Determine the type of a boost python object
 */ 
PythonType DetermineType(const boost::python::object & obj);




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
        return conv.check() && CustomCheck(obj);
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


    /*! \brief Prevents some implicit conversions
     */
    static bool CustomCheck(const boost::python::object & obj)
    {
        PythonType objtype = DetermineType(obj);

        // don't promote bool to int or float
        if(objtype == PythonType::Bool)
        {
            if(std::is_same<T, int>::value)
                return false;
            if(std::is_same<T, float>::value)
                return false;
            if(std::is_same<T, double>::value)
                return false;
        }

        // don't promote int or float to bool
        if(objtype == PythonType::Int || objtype == PythonType::Float)
        {
            if(std::is_same<T, bool>::value)
                return false;
        }

        // don't promote float to int
        // (shouldn't be possible from boost::python::extract, but make sure)
        if(objtype == PythonType::Float)
        {
            if(std::is_same<T, int>::value)
                return false;
        }

        return true;
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
            catch(exception::GeneralException & ex)
            {
                ex.AppendInfo("element", std::to_string(i));
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
 * \throw exception::PythonConvertException if the
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
        throw exception::GeneralException("Cannot convert from python to C++: Check failed",
                               "fromtype", fromtype,
                               "totype", typeid(T).name());
    }

    try {
        return ToCppConverter<T>::Convert(obj);
    }
    catch(...) //! \todo Doesn't seem to catch python exceptions?
    {
        std::string fromtype = boost::python::extract<std::string>(obj.attr("__class__").attr("__name__"));
        throw exception::GeneralException("Cannot convert from python to C++: Conversion failed",
                               "fromtype", fromtype,
                               "totype", typeid(T).name());
    }
}




/*! \brief Check if a conversion of a boost::python::object to a C++ type is valid
 *
 * This wraps the ToCppConverter structures.
 *
 * \tparam T The type to convert to
 *
 * \param [in] obj The object to convert
 * \return Converted data as type T
 */
template<typename T>
bool TestConvertToCpp(const boost::python::object & obj)
{
    return ToCppConverter<T>::Check(obj);
}



/*! \brief Convert a C++ object to a boost::python::object
 *
 * \throw exception::PythonConvertException if the
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
        throw exception::GeneralException("Cannot convert from C++ to python: Conversion failed",
                               "fromtype", typeid(obj).name(),
                               "totype", "boost::python::object");
    }
}



/*! \brief Test conversion of a C++ object to a boost::python::object
 *
 * \tparam T The type to convert from
 *
 * \todo better way to do this?
 *
 * \param [in] obj The object to convert
 * \return Converted data as a boost::python::object
 */
template<typename T>
bool TestConvertToPy(const T & obj)
{
    try {
        boost::python::object & ret = boost::python::object(obj); 
    }
    catch(...) //! \todo limit this
    {
        return false;
    }

    return true;
}





/*! \brief Convert a C++ vector to a boost::python::list
 *
 * \throw exception::PythonConvertException if the
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
        catch(exception::GeneralException & ex)
        {
            ex.AppendInfo("element", std::to_string(i));
            throw;
        }
    }

    return result;
}



} // close namespace python_helper
} // close namespace bpmodule

#endif
