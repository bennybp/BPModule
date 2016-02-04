#ifndef BPMODULE_GUARD_PYTHON__PYBIND11_STL_HPP_
#define BPMODULE_GUARD_PYTHON__PYBIND11_STL_HPP_

#include "bpmodule/pragma.h"

PRAGMA_WARNING_PUSH
PRAGMA_WARNING_IGNORE_SHADOW
PRAGMA_WARNING_IGNORE_SHADOW_MEMBER
PRAGMA_WARNING_IGNORE_EXTRA_SEMICOLON
PRAGMA_WARNING_IGNORE_SWITCH_MISSING_DEFAULT
PRAGMA_WARNING_IGNORE_FP_EQUALITY
PRAGMA_WARNING_IGNORE_GCC_PRAGMA
PRAGMA_WARNING_IGNORE_NONVIRTUAL_DTOR
#include "pybind11/stl.h"




NAMESPACE_BEGIN(pybind11)
NAMESPACE_BEGIN(detail)

template <typename Value, size_t N> struct type_caster<std::array<Value, N>> {
    typedef std::array<Value, N> type;
    typedef type_caster<Value> value_conv;
public:
    bool load(handle src, bool convert) {
        list l(src, true);
        if (!l.check())
            return false;
        size_t size = l.size();
        if(size != N)
            return false;
        value_conv conv;
        size_t idx = 0;
        for (auto it : l) {
            if (!conv.load(it, convert))
                return false;
            value[idx++] = (Value) conv;
        }
        return true;
    }

    static handle cast(const type &src, return_value_policy policy, handle parent) {
        list l(src.size());
        size_t index = 0;
        for (auto const &value2: src) {
            object value_ = object(value_conv::cast(value2, policy, parent), false);
            if (!value_)
                return handle();
            PyList_SET_ITEM(l.ptr(), index++, value_.release().ptr()); // steals a reference
        }
        return l.release();
    }
    PYBIND11_TYPE_CASTER(type, _("list<") + value_conv::name() + _(">"));
};


NAMESPACE_END(detail)
NAMESPACE_END(pybind11)

PRAGMA_WARNING_POP

#endif
