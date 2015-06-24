#ifndef PYTHON_CONVERT_H
#define PYTHON_CONVERT_H

#include <vector>
#include <string>

#include <boost/python.hpp>

namespace bpmodule {
namespace export_python {


template<typename T>
static
std::vector<T> ConvertListToVec(const boost::python::list & list)
{
    int length = boost::python::extract<int>(list.attr("__len__")());
    std::vector<T> ret;
    ret.reserve(length);

    for (int i = 0; i < length; i++)
        ret.push_back(static_cast<T>(boost::python::extract<T>(list[i])));

    return ret;
}

/*
template<typename T>
static 
T ConvertDictToMap(const dict & d)
{
    T m;
    int length = boost::python::extract<int>(list.attr("__len__")());
    for(int i = 0; i < length; i++)
    

    return m;
}
*/

template<typename T>
boost::python::list ConvertVecToList(const std::vector<T> & v)
{
    boost::python::list result;
    for(auto & it : v)
        result.append(it);    
    return result;
}

template<typename T>
boost::python::dict ConvertMapToDict(const T & m)
{
    boost::python::dict d;
    for(auto & it : m)
        d[it.first] = it.second;
    return d;
}




/////////////////////////////////////////
// Structures for python converters
/////////////////////////////////////////
template<typename T>
struct VectorConverter
{
    static PyObject* convert(const std::vector<T> & v)
    {
      boost::python::list lst = ConvertVecToList(v);
      return boost::python::incref(lst.ptr());
    }
};


template<class T1, class T2>
struct PairConverter
{
    static PyObject* convert(const std::pair<T1, T2>& pair)
    {
      return boost::python::incref(boost::python::make_tuple(pair.first, pair.second).ptr());
    }
};

template<typename T>
struct MapConverter
{
    static PyObject* convert(const T & m)
    {
      boost::python::dict d = ConvertMapToDict(m);
      return boost::python::incref(d.ptr());
    }
};



} // close namespace export_python
} // close namespace bpmodule

#endif
