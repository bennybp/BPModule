/*! \file
 *
 * \brief Conversion between python and C++ (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_CONVERT_HPP_
#define _GUARD_CONVERT_HPP_

#include <boost/python/list.hpp>  // includes object
#include <boost/python/extract.hpp>

#include "bpmodule/python_helper/Types.hpp"
#include "bpmodule/python_helper/Errors.hpp"
#include "bpmodule/exception/PythonConvertException.hpp"
#include "bpmodule/mangle/Mangle.hpp"


namespace bpmodule {
namespace python_helper {




/*! \brief Family of structs to convert python types to and from C++ types
 *
 * \tparam T C++ type to convert
 */
template<typename T>
struct PyConverter
{
    /*! \brief Converts a python object to type \p T
     *
     * \throw bpmodule::exception::PythonConvertException
     *        if there is a problem with the conversion
     *
     * \param [in] obj Object to convert
     */
    static T ConvertToCpp(const boost::python::object & obj)
    {
        // check for some disallowed conversions
        if(!TypeCheck(obj))
            throw exception::PythonConvertException("Cannot convert from python to C++: Incompatible types",
                                                    GetPyClass(obj), mangle::DemangleCppType<T>());

        /*
         * The extract below may throw a few different errors. In particular,
         * it may throw objects derived from std::bad_cast
         * which are thrown from boost::numeric. These
         * indicate overflows and underflows.
         */
        try {
            return boost::python::extract<T>(obj);
        }
        catch(const std::exception & ex)
        {
            throw exception::PythonConvertException("Cannot convert from python to C++: Conversion failed",
                                                    GetPyClass(obj), mangle::DemangleCppType<T>(),
                                                    "stdex", ex.what());
        }
        catch(...)
        {
            std::string exstr = detail::GetPyExceptionString();
            throw exception::PythonConvertException("Cannot convert from python to C++: Conversion failed",
                                                    GetPyClass(obj), mangle::DemangleCppType<T>(),
                                                    "pyex", exstr);
        }
    }


    /*! \brief Convert a C++ object to a boost::python::object
     *
     * \throw exception::PythonConvertException if the
     *        data could not be converted
     *
     * \param [in] obj The object to convert
     * \return Converted data as a boost::python::object
     */
    static boost::python::object ConvertToPy(const T & obj)
    {
        try {
           return boost::python::object(obj);
        }
        catch(...)
        {
            std::string exstr = detail::GetPyExceptionString();
            throw exception::PythonConvertException("Cannot convert from C++ to python: Conversion failed",
                                                    mangle::DemangleCppType(obj), "boost::python::object",
                                                    "pythonex", exstr);
        }
    }




    /*! \brief Prevents some implicit conversions
     *
     * Used to deny implicit conversions from int to floating point, etc.
     */
    static bool TypeCheck(const boost::python::object & obj)
    {
        PythonType objtype = DeterminePyType(obj);

        // don't promote bool to anything else
        if(!std::is_same<T, bool>::value && objtype == PythonType::Bool)
            return false;

        // don't promote anything to bool
        if(std::is_same<T, bool>::value && objtype != PythonType::Bool)
            return false;

        // don't promote integer to floating point (or anything else)
        if(!std::is_integral<T>::value && objtype == PythonType::Int)
            return false;

        // also don't promote floating point to integral types
        // (shouldn't be possible from boost::python::extract, but make sure)
        if(!std::is_floating_point<T>::value && objtype == PythonType::Float)
            return false;

        return true;
    }
};





/*! \brief Converts a python list to a std::vector
 *
 * \tparam T Type of the vector element to convert
 */
template<typename T>
struct PyConverter<std::vector<T>>
{
    /*! \brief Converts a python object to type std::vector<T>
     *
     * \throw bpmodule::exception::PythonConvertException
     *        if there is a problem with the conversion
     *
     * \param [in] obj Object to convert
     */
    static std::vector<T> ConvertToCpp(const boost::python::object & obj)
    {
        std::vector<T> r;

        boost::python::extract<boost::python::list> lconv(obj);
        if(!lconv.check())
            throw exception::PythonConvertException("Cannot convert from python to C++ vector: Object is not a list",
                                                    GetPyClass(obj), mangle::DemangleCppType<std::vector<T>>());

        boost::python::list lst = lconv();

        int length = boost::python::extract<int>(lst.attr("__len__")());
        if(length == 0)
            return r;

        r.reserve(length);

        for (int i = 0; i < length; i++)
        {
            // catch exceptions and add the element index
            try {
                r.push_back(PyConverter<T>::ConvertToCpp(lst[i]));
            }
            catch(exception::PythonConvertException & ex)
            {
                ex.AppendInfo("vecto", mangle::DemangleCppType<std::vector<T>>(),
                              "element", std::to_string(i));
                throw;
            }
        }

        return r;

    }


    /*! \brief Convert a C++ vector to a boost::python::object (list)
     *
     * \throw exception::PythonConvertException if the
     *        data could not be converted
     *
     * \param [in] v The vector to convert
     * \return Converted data as a boost::python::list
     */
    static boost::python::object ConvertToPy(const std::vector<T> & v)
    {
        boost::python::list result;

        for(size_t i = 0; i < v.size(); ++i)
        {
            try {
                result.append(PyConverter<T>::ConvertToPy(v[i]));
            }
            catch(exception::PythonConvertException & ex)
            {
                ex.AppendInfo("vecfrom", mangle::DemangleCppType<std::vector<T>>(),
                              "element", std::to_string(i));
                throw;
            }
        }

        return boost::python::object(result);
    }
};







/*! \brief Convert a boost::python::object to a C++ type
 *
 * \throw exception::PythonConvertException if the
 *        data could not be converted
 *
 * \tparam T The C++ type to convert to
 *
 * \param [in] obj The object to convert
 * \return Converted data as type \p T
 */
template<typename T>
T ConvertToCpp(const boost::python::object & obj)
{
    // will throw if there is an issue
    return PyConverter<T>::ConvertToCpp(obj);
}




/*! \brief Convert a C++ object to a boost::python::object
 *
 * \throw exception::PythonConvertException if the
 *        data could not be converted
 *
 * \tparam T The C++ type to convert from
 *
 * \param [in] obj The object to convert
 * \return Converted data as a boost::python::object
 */
template<typename T>
boost::python::object ConvertToPy(const T & obj)
{
    // will throw if there is an issue
    return PyConverter<T>::ConvertToPy(obj);
}





/*! \brief Use to create to_python converters
 *
 * This uses the ConvertToPy function from this file.
 *
 * Particularly, use this to export a vector
 * of class objects that have already been
 * exported to python.
 *
 * \throw exception::PythonConvertException if the
 *        data could not be converted
 */
template<typename T>
struct CppToPyExport
{
    static PyObject * convert(const T & v)
    {
        return boost::python::incref(ConvertToPy(v).ptr());
    }  
};

} // close namespace python_helper
} // close namespace bpmodule

#endif
