#ifndef _GUARD_PYBIND11_STL_HPP_
#define _GUARD_PYBIND11_STL_HPP_

#include "bpmodule/pragma.h"

PRAGMA_WARNING_PUSH
PRAGMA_WARNING_IGNORE_SHADOW
PRAGMA_WARNING_IGNORE_EXTRA_SEMICOLON
PRAGMA_WARNING_IGNORE_SWITCH_MISSING_DEFAULT
PRAGMA_WARNING_IGNORE_FP_EQUALITY
#include "pybind11/stl.h"




NAMESPACE_BEGIN(pybind11)
NAMESPACE_BEGIN(detail)
template <typename Value, size_t N> struct type_caster<std::array<Value, N>> {
    typedef std::array<Value, N> type;
    typedef type_caster<Value> value_conv;
public:
    bool load(PyObject *src, bool convert) {
        if (!PyList_Check(src))
            return false;
        size_t size = (size_t) PyList_GET_SIZE(src);
        if(size != N)
            return false;
        value_conv conv;
        for (size_t i=0; i<size; ++i) {
            if (!conv.load(PyList_GetItem(src, (ssize_t) i), convert))
                return false;
            value[i] = (Value) conv;
        }
        return true;
    }

    static PyObject *cast(const type &src, return_value_policy policy, PyObject *parent) {
        PyObject *list = PyList_New(src.size());
        size_t index = 0;
        for (auto const &value: src) {
            PyObject *value_ = value_conv::cast(value, policy, parent);
            if (!value_) {
                Py_DECREF(list);
                return nullptr;
            }
            PyList_SetItem(list, index++, value_);
        }
        return list;
    }
    PYBIND11_TYPE_CASTER(type, detail::descr("list<") + value_conv::name() + detail::descr(">"));
};


NAMESPACE_END(detail)
NAMESPACE_END(pybind11)






PRAGMA_WARNING_POP

#endif
