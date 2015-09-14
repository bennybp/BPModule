#ifndef _GUARD_PYTHON_STDCONVERT_HPP_
#define _GUARD_PYTHON_STDCONVERT_HPP_

#include "bpmodule/python_helper/Convert.hpp"

namespace bpmodule {
namespace python_helper {


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
            throw ex;
        }
    }

    return ret;
}


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
            throw ex;
        }
    }

    return result;
}



template<typename T1, typename T2>
boost::python::tuple ConvertPairToTuple(const std::pair<T1, T2> & m)
{
    try {
        boost::python::object first = ConvertToPy(m.first);
        boost::python::object second = ConvertToPy(m.second);
        return boost::python::make_tuple(first, second);
    }
    catch(bpmodule::exception::PythonConvertException & ex)
    {
        ex.AppendInfo({{ "location", "ConvertPairToTuple"}});
        throw ex;
    }
}



/////////////////////////////////////////
// Structures for python converters
/////////////////////////////////////////
/*
template<typename T>
struct VectorConverter
p
    static PyObject* convert(const std::vector<T> & v)
    {
        // may throw
        boost::python::list lst = ConvertVecToList(v);
        return boost::python::incref(lst.ptr());
    }
};


template<class T1, class T2>
struct PairConverter
{
    static PyObject* convert(const std::pair<T1, T2>& pair)
    {
        // may throw
        boost::python::tuple t = ConvertPairToTuple(pair);
        return boost::python::incref(t.ptr());
    }
};
*/

} // close namespace python_helper
} // close namespace bpmodule

#endif
