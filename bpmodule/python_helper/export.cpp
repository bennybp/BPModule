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



BOOST_PYTHON_MODULE(python_helper)
{
    // implicit conversions of vectors
    to_python_converter<std::vector<signed char>,         CppToPyExport<std::vector<signed char>>>();
    to_python_converter<std::vector<unsigned char>,       CppToPyExport<std::vector<unsigned char>>>();
    to_python_converter<std::vector<signed short>,        CppToPyExport<std::vector<signed short>>>();
    to_python_converter<std::vector<unsigned short>,      CppToPyExport<std::vector<unsigned short>>>();
    to_python_converter<std::vector<signed int>,          CppToPyExport<std::vector<signed int>>>();
    to_python_converter<std::vector<unsigned int>,        CppToPyExport<std::vector<unsigned int>>>();
    to_python_converter<std::vector<signed long>,         CppToPyExport<std::vector<signed long>>>();
    to_python_converter<std::vector<unsigned long>,       CppToPyExport<std::vector<unsigned long>>>();
    to_python_converter<std::vector<signed long long>,    CppToPyExport<std::vector<signed long long>>>();
    to_python_converter<std::vector<unsigned long long>,  CppToPyExport<std::vector<unsigned long long>>>();

    // implicit conversions of some std::arrays
    to_python_converter<std::array<float, 3>,              CppToPyExport<std::array<float, 3>>>();
    to_python_converter<std::array<double, 3>,             CppToPyExport<std::array<double, 3>>>();
    to_python_converter<std::array<long double, 3>,        CppToPyExport<std::array<long double, 3>>>();


    to_python_converter<std::vector<float>,               CppToPyExport<std::vector<float>>>();
    to_python_converter<std::vector<double>,              CppToPyExport<std::vector<double>>>();
    to_python_converter<std::vector<long double>,         CppToPyExport<std::vector<long double>>>();

}


} // close namespace export_python
} // close namespace python_helper
} // close namespace bpmodule

