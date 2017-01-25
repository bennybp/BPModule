#include "pulsar/util/PythonHelper.hpp"

using namespace pybind11;
namespace pulsar {

template<typename T>
bool container_pickleable(const object& obj)
{
    T tmp=obj.cast<T>();
    for(const auto& i:tmp)
        if(!pulsar::is_pickleable(reinterpret_borrow<object>(i)))
            return false;
    return true;
}



/* To be pickleable:
 * None, True, and False
 * integers, floating point numbers, complex numbers
 * strings, bytes, bytearrays
 * tuples, lists, sets, and dictionaries containing only picklable objects
 * functions defined at the top level of a module (using def, not lambda)
 * built-in functions defined at the top level of a module
 * classes that are defined at the top level of a module
 * instances of such classes whose __dict__ or the result of calling __getstate__() is picklable
 *
 * As far as I can tell:
 *    - There is no pybind11::bytearray
 *
 */

bool is_pickleable(const object& obj)
{
    if(isinstance<pybind11::none>(obj) || isinstance<pybind11::bool_>(obj) ||
       isinstance<pybind11::int_>(obj) || isinstance<pybind11::float_>(obj)||
       isinstance<pybind11::str>(obj)  || isinstance<pybind11::bytes>(obj))
        return true;
    if(isinstance<pybind11::list>(obj))
        return container_pickleable<pybind11::list>(obj);
    if(isinstance<pybind11::set>(obj))
        return container_pickleable<pybind11::set>(obj);
    if(isinstance<pybind11::tuple>(obj))
        return container_pickleable<pybind11::tuple>(obj);
    if(isinstance<dict>(obj))
    {
        dict tmp=obj.cast<dict>();
        for(const auto& i:tmp)
        {
            object o1=reinterpret_borrow<object>(i.first),
                   o2=reinterpret_borrow<object>(i.second);
            if(!(is_pickleable(o1)||is_pickleable(o2)))
                return false;
        }
        return true;
    }
    if(has_attr(obj,"__getstate__") && has_attr(obj,"__setstate__"))
        return is_pickleable(call_py_func_attr<object>(obj,"__getstate__"));
//    if(has_attr(obj,"__dict__"))
//        return is_pickleable(call_py_func_attr<object>(obj,"__dict__"));
    return false;
}

}//End namespace pulsar
