/*! \file
 *
 * \brief A smart pointer to a module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULELOCATOR__MODULEPTR_HPP_
#define BPMODULE_GUARD_MODULELOCATOR__MODULEPTR_HPP_

#include "bpmodule/modulelocator/ModuleIMPLHolder.hpp"

namespace bpmodule {
namespace modulelocator {

/*! \brief A C++ smart pointer containing a module
 *
 * This is the main object used for manipulating a module and
 * calling its functions. It behaves like a smart pointer that
 * cannot be copied or copy assigned.
 *
 * Functions of the contained module are called via the -> operator,
 * similar to a smart pointer.
 *
 * \tparam T Type of module held (module base class)
 */
template<typename T>
class ModulePtr
{
    public:
        /*! \brief Constructor from an IMPL holder rvalue reference
         */
        ModulePtr(std::unique_ptr<detail::ModuleIMPLHolder> && holder)
                : holder_(holder.release())
        {
            using namespace bpmodule::exception;

            Assert<GeneralException>((bool)holder_, "ModulePtr given a null pointer");

            ptr_ = dynamic_cast<T *>(holder_->CppPtr());
            Assert<GeneralException>(ptr_ != nullptr, "ModulePtr not given a holder of the right type");
        }


        ModulePtr(ModulePtr && /*rhs*/) = default;
        ModulePtr & operator=(ModulePtr && /*rhs*/) = default;

        // no copy construction or assignment
        ModulePtr(const ModulePtr & /*rhs*/) = delete;
        ModulePtr & operator=(const ModulePtr & /*rhs*/) = delete;

        ~ModulePtr() = default; // actual deletion done by unique_ptr

        /*! \brief Dereference the object
         * 
         * Used to call functions of the contained module object
         *
         * \exnothrow
         */ 
        T * operator->() const { return ptr_; }


        /*! \brief Dereference the object
         *
         * Used to call functions of the contained module object
         *
         * \exnothrow
         */ 
        T & operator*() const { return *ptr_; }


    private:
        //! The acutal held module
        std::unique_ptr<detail::ModuleIMPLHolder> holder_;

        //! C++ pointer to the held module
        T * ptr_;
};



/*! \brief A C++ smart pointer containing a module
 *
 * This is a python version of ModulePtr. Like ModulePtr,
 * it contains a module and acts similar to a C++ smart pointer,
 * allowing python to access attributes/members of the held object.
 *
 * See \ref python_smart_pointer
 */
class PyModulePtr
{
    public:
        /*! \brief Constructor from an IMPL holder rvalue reference
         */
        PyModulePtr(std::unique_ptr<detail::ModuleIMPLHolder> && holder)
                  : holder_(holder.release()), obj_(holder_->PythonObject())
        {
            using namespace bpmodule::exception;
            Assert<GeneralException>((bool)holder_, "PyModulePtr given a null pointer");
            Assert<GeneralException>((bool)obj_, "PyModulePtr could not convert to python object");
        }

        PyModulePtr(PyModulePtr && /*rhs*/) = default;
        PyModulePtr & operator=(PyModulePtr && /*rhs*/) = default;
        PyModulePtr & operator=(const PyModulePtr & /*rhs*/) = delete;

        // This must be copyable to be used from python
        //! \todo Is that true?
        PyModulePtr(const PyModulePtr & /*rhs*/) = default;

        ~PyModulePtr() = default;


        /*! \brief Used by python for smart pointer semantics
         * 
         * See \ref python_smart_pointer
         *
         * \note May raise a python exception if the specified method doesn't exist
         */
        pybind11::object Py__getattr__(const std::string & name)
        {
            return obj_.attr(name.c_str());
        }



    private:
        //! The acutal held module
        std::shared_ptr<detail::ModuleIMPLHolder> holder_;

        //! Held module converted to a pybind11::object
        pybind11::object obj_;
};



} // close namespace modulelocator
} // close namespace bpmodule


#endif

