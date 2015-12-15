#ifndef _GUARD_MODULEPTR_HPP_
#define _GUARD_MODULEPTR_HPP_

#include "bpmodule/modulelocator/ModuleIMPLHolder.hpp"

namespace bpmodule {
namespace modulelocator {

template<typename T>
class ModulePtr
{
    public:
        // takes ownership
        ModulePtr(std::unique_ptr<detail::ModuleIMPLHolder> && holder)
                : holder_(std::move(holder))
        {
            //! \todo check dynamic cast
            ptr_ = dynamic_cast<T *>(holder_->CppPtr());
        }

        ModulePtr(ModulePtr && rhs) = default;
        ModulePtr & operator=(ModulePtr && rhs) = default;

        ModulePtr(const ModulePtr & rhs) = delete;
        ModulePtr & operator=(const ModulePtr & rhs) = delete;

        ~ModulePtr() = default;

        T & operator->() const { return *ptr_; }

        T * operator*() const { return ptr_; }


    private:
        std::unique_ptr<detail::ModuleIMPLHolder> holder_;
        T * ptr_;  // for convenience
};



class PyModulePtr
{
    public:
        // takes ownership
        PyModulePtr(std::unique_ptr<detail::ModuleIMPLHolder> && holder)
                  : holder_(holder.release()), obj_(holder_->PythonObject())
        {
            //! \todo check for null ptr 
        }

        PyModulePtr(PyModulePtr && rhs) = default;
        PyModulePtr & operator=(PyModulePtr && rhs) = default;

        // these are not const since the copy constructor
        // and operator= for pybind11::object are not const
        PyModulePtr(PyModulePtr & rhs) = default;
        PyModulePtr & operator=(PyModulePtr & rhs) = default;

        ~PyModulePtr() = default;


        pybind11::object Py__getattr__(const std::string & name)
        {
            return obj_.attr(name.c_str());
        }



    private:
        std::shared_ptr<detail::ModuleIMPLHolder> holder_;
        pybind11::object obj_;  // for convenience
};



} // close namespace modulelocator
} // close namespace bpmodule


#endif

