/*! \file
 *
 * \brief A smart pointer to a module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULEMANAGER__MODULEPTR_HPP_
#define BPMODULE_GUARD_MODULEMANAGER__MODULEPTR_HPP_

#include "bpmodule/modulemanager/ModuleIMPLHolder.hpp"

namespace bpmodule {
namespace modulemanager {

/*! \brief A C++ smart pointer containing a module
 *
 * This is the main object used for manipulating a module and
 * calling its functions. It behaves like a smart pointer that
 * cannot be copied or copy assigned.
 *
 * Functions of the contained module are called via the -> operator,
 * similar to a smart pointer.
 *
 * \tparam T Type of module held (a module base class)
 */
template<typename T>
class ModulePtr
{
    public:
        /*! \brief Constructor from an IMPL holder rvalue reference
         */
        ModulePtr(std::unique_ptr<detail::ModuleIMPLHolder> && holder)
                : holder_(std::move(holder))
        {
            using namespace bpmodule::exception;

            // check before getting Cpp pointer
            Assert<GeneralException>((bool)holder_, "ModulePtr given a null pointer");

            ptr_ = dynamic_cast<T *>(holder_->CppPtr());

            // Attempt to convert to the proper pointer type
            Assert<GeneralException>(ptr_ != nullptr, "ModulePtr not given a holder of the right type");
        }


        ModulePtr(ModulePtr &&) = default;
        ModulePtr & operator=(ModulePtr &&) = default;

        // no copy construction or assignment
        ModulePtr(const ModulePtr &) = delete;
        ModulePtr & operator=(const ModulePtr &) = delete;

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
        //! The acutal held module (pointer to the ModuleIMPLHolder base class)
        std::unique_ptr<detail::ModuleIMPLHolder> holder_;

        //! C++ pointer to the held module
        T * ptr_;
};



/*! \brief A Python object containing a module
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
        /*! \brief Constructor
         *
         * Moves the held module from the unique pointer 
         *
         * \param [in] holder The holder of the module to be contained in 
         *                    this object
         */
        PyModulePtr(std::unique_ptr<detail::ModuleIMPLHolder> && holder)
                  : holder_(std::move(holder))
        {
            using namespace bpmodule::exception;

            // check before getting the python object
            Assert<GeneralException>((bool)holder_, "PyModulePtr given a null pointer");
            obj_ = holder_->PythonObject();

            Assert<GeneralException>((bool)obj_, "Module given to PyModulePtr could not be converted to python object");
        }

        PyModulePtr(PyModulePtr &&) = default;
        PyModulePtr & operator=(PyModulePtr &&) = default;
        PyModulePtr & operator=(const PyModulePtr &) = delete;
        PyModulePtr(const PyModulePtr &) = delete;

        ~PyModulePtr() = default;


        /*! \brief Dereference the module. Used by python for smart pointer semantics
         * 
         * See \ref python_smart_pointer
         *
         * \note May raise a python exception if the specified method doesn't exist
         *
         * \param [in] name The attribute to get from the module
         */
        pybind11::object Py__getattr__(const std::string & name)
        {
            return obj_.attr(name.c_str());
        }



    private:
        //! The acutal held module
        std::unique_ptr<detail::ModuleIMPLHolder> holder_;

        //! Held module converted to a pybind11::object
        pybind11::object obj_;
};



} // close namespace modulemanager
} // close namespace bpmodule


#endif

