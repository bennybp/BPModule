/*! \file
 *
 * \brief Conversion between python and C++ (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_CONVERT_HPP_
#define _GUARD_CONVERT_HPP_

#include <boost/python.hpp>

#include "bpmodule/python_helper/Types.hpp"
#include "bpmodule/exception/PythonConvertException.hpp"


namespace bpmodule {
namespace python_helper {


inline std::string GetPyExceptionString(void)
{
    //! \todo leaking memory?
    if(PyErr_Occurred() != NULL)
    {
        PyObject *e, *v, *t;
        PyErr_Fetch(&e, &v, &t);

        boost::python::object e_obj(boost::python::handle<>(boost::python::allow_null(e)));
        boost::python::object v_obj(boost::python::handle<>(boost::python::allow_null(v)));
        boost::python::object t_obj(boost::python::handle<>(boost::python::allow_null(t)));
       
        std::string errstr = boost::python::extract<std::string>(e_obj);
        errstr += " : ";
        errstr += boost::python::extract<std::string>(v_obj);
        return errstr;
    }
    else
        return "(no error?)";
}






/*! \brief Family of structs to convert python types to C++ types
 *
 * \tparam T Type to convert to
 */
template<typename T>
struct ToCppConverter
{
    /*! \brief Checks if a python object is convertable to type T
     *
     * \note Does not check for arithmetic underflows, overflows, etc
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
     * \throw bpmodule::exception::PythonConvertException
     *        if there is a problem with the conversion
     *
     * \param [in] obj Object to check
     */ 
    static T Convert(const boost::python::object & obj)
    {
        /* The below may throw a few different errors. In particular,
         * it may throw objects derived from std::bad_cast
         * which are thrown from boost::numeric. These
         * indicate overflows
         */ 
        try {
            boost::python::extract<T> conv(obj);
            return conv();
        }
        catch(const std::exception & ex)
        {
            throw exception::PythonConvertException("Cannot convert from python to C++: Conversion failed",
                                                    GetPyClass(obj), typeid(T).name(),
                                                    "stdex", ex.what());
        }
        catch(...)
        {
            std::string exstr = GetPyExceptionString();
            throw exception::PythonConvertException("Cannot convert from python to C++: Conversion failed",
                                                    GetPyClass(obj), typeid(T).name(),
                                                    "pyex", exstr);
        }
    }


    /*! \brief Prevents some implicit conversions
     *
     * Used to deny implicit conversions from int to floating point, etc. 
     */
    static bool CustomCheck(const boost::python::object & obj)
    {
        PythonType objtype = DeterminePyType(obj);

        // don't promote bool to anything else
        if(objtype == PythonType::Bool)
        {
            if(!std::is_same<T, bool>::value)
                return false;
        }

        // don't promote anything to bool
        if(std::is_same<T, bool>::value && objtype != PythonType::Bool)
            return false;

        // don't promote integer to floating point (or anything else)
        if(objtype == PythonType::Int && !std::is_integral<T>::value)
            return false;

        // also don't promote floating point to integral types
        // (shouldn't be possible from boost::python::extract, but make sure)
        if(objtype == PythonType::Float && !std::is_floating_point<T>::value)
            return false;

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
     * \throw bpmodule::exception::PythonConvertException
     *        if there is a problem with the conversion
     *
     * \param [in] obj Object to convert
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
            catch(exception::PythonConvertException & ex)
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
        throw exception::PythonConvertException("Cannot convert from python to C++: Check failed",
                                                GetPyClass(obj), typeid(T).name());
    }

    // will throw if there is an issue
    return ToCppConverter<T>::Convert(obj);
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
        std::string exstr = GetPyExceptionString();
        throw exception::PythonConvertException("Cannot convert from C++ to python: Conversion failed",
                                                typeid(obj).name(), "boost::python::object",
                                                "pythonex", exstr);
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
        boost::python::object ret = boost::python::object(obj); 
    }
    catch(...) //! \todo limit this?
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
        catch(exception::PythonConvertException & ex)
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
