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

/*! \brief A smart pointer containing a module
 */
template<typename T>
class ModulePtr
{
    public:
        /*! \brief Constructor from an IMPL holder
         * 
         * Takes ownership of \p holder
         */
        ModulePtr(std::unique_ptr<detail::ModuleIMPLHolder> && holder)
                : holder_(holder.release())
        {
            using namespace bpmodule::exception;


            ptr_ = dynamic_cast<T *>(holder_->CppPtr());
            Assert<GeneralException>(ptr_ != nullptr, "ModulePtr not given a holder of the right type");
        }


        ModulePtr(ModulePtr && rhs) = default;
        ModulePtr & operator=(ModulePtr && rhs) = default;

        // no copy construction or assignment
        ModulePtr(const ModulePtr & rhs) = delete;
        ModulePtr & operator=(const ModulePtr & rhs) = delete;

        ~ModulePtr() = default;

        /*! \brief Dereference the object
         */ 
        T * operator->() const { return ptr_; }

        /*! \brief Dereference the object
         */ 
        T & operator*() const { return *ptr_; }


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
        PyModulePtr(const PyModulePtr & rhs) = default;

        //! \todo bug in pybind11? must take non-const because obj_::operator= takes non const
        
        //PyModulePtr & operator=(const PyModulePtr & rhs) = default;

        ~PyModulePtr() = default;


        /*! \brief Used by python for smart pointer semantics
         * 
         * See \ref python_smart_pointer
         *
         * \note May raise a python exception if the sepcified method doesn't exist
         */
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

