/*! \file
 *
 * \brief Python exports for python helper functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include <vector>
#include <boost/python/module.hpp>
#include <boost/python/to_python_converter.hpp>

#include "bpmodule/python_helper/Convert.hpp"

using namespace boost::python;


namespace bpmodule {
namespace python_helper {
namespace export_python {


/////////////////////////////////
// Some to-python conveters
// These all use the functions
// from Convert.hpp
/////////////////////////////////
template<typename T>
struct CppToPy_
{
    static PyObject * convert(const T & v)
    {
        return incref(ConvertToPy(v).ptr());
    }  
};




BOOST_PYTHON_MODULE(python_helper)
{
    // implicit conversions of vectors
    to_python_converter<std::vector<signed char>,         CppToPy_<std::vector<signed char>>>();
    to_python_converter<std::vector<unsigned char>,       CppToPy_<std::vector<unsigned char>>>();
    to_python_converter<std::vector<signed short>,        CppToPy_<std::vector<signed short>>>();
    to_python_converter<std::vector<unsigned short>,      CppToPy_<std::vector<unsigned short>>>();
    to_python_converter<std::vector<signed int>,          CppToPy_<std::vector<signed int>>>();
    to_python_converter<std::vector<unsigned int>,        CppToPy_<std::vector<unsigned int>>>();
    to_python_converter<std::vector<signed long>,         CppToPy_<std::vector<signed long>>>();
    to_python_converter<std::vector<unsigned long>,       CppToPy_<std::vector<unsigned long>>>();
    to_python_converter<std::vector<signed long long>,    CppToPy_<std::vector<signed long long>>>();
    to_python_converter<std::vector<unsigned long long>,  CppToPy_<std::vector<unsigned long long>>>();

    to_python_converter<std::vector<float>,               CppToPy_<std::vector<float>>>();
    to_python_converter<std::vector<double>,              CppToPy_<std::vector<double>>>();
    to_python_converter<std::vector<long double>,         CppToPy_<std::vector<long double>>>();

}


} // close namespace export_python
} // close namespace python_helper
} // close namespace bpmodule

