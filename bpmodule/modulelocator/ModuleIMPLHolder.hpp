#ifndef _GUARD_MODULEIMPLHOLDER_HPP_
#define _GUARD_MODULEIMPLHOLDER_HPP_

#include <memory>

#include "bpmodule/python_helper/Pybind11.hpp"
#include "bpmodule/exception/GeneralException.hpp"


namespace bpmodule {
namespace modulebase {
class ModuleBase;
}
}


namespace bpmodule {
namespace modulelocator {
namespace detail {



class ModuleIMPLHolder
{
    public:
        virtual ~ModuleIMPLHolder() = default;

        virtual modulebase::ModuleBase * CppPtr(void) = 0;
        virtual pybind11::object PythonObject(void) = 0;
};



// T should hold pointer to base type, but no further
template<typename T>
class CppModuleIMPLHolder : public ModuleIMPLHolder
{
    public:
        CppModuleIMPLHolder(std::unique_ptr<T> && mod) : mod_(std::move(mod)) { }

        virtual modulebase::ModuleBase * CppPtr(void)
        {
            return mod_.get(); 
        }

        virtual pybind11::object PythonObject(void)
        {
            T * ptr = dynamic_cast<typename T::BaseType *>(mod_.get());
            pybind11::object o = pybind11::cast(ptr, pybind11::return_value_policy::reference);

            //! \todo replace with exception if pybind11 does
            if(o.ptr() == nullptr)
                throw exception::GeneralException("Unable to cast to python object: got a null ptr");

            return o;
        } 


    private:
        std::unique_ptr<T> mod_;
};




class PyModuleIMPLHolder : public ModuleIMPLHolder
{
    public:
        //! \todo make this a move constructor?
        PyModuleIMPLHolder(pybind11::object mod) : mod_(mod) { }

        //! \todo checking? Exceptions or mod_.ptr() == nullptr
        virtual modulebase::ModuleBase * CppPtr(void) { return mod_.cast<modulebase::ModuleBase *>(); }

        virtual pybind11::object PythonObject(void) { return mod_; }

    private:
        pybind11::object mod_;
};



} // close namespace detail
} // close namespace modulelocator
} // close namespace bpmodule

#endif

