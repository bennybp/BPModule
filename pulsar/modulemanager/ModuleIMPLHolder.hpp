/*! \file
 *
 * \brief Implementations for module smart pointers (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEMANAGER__MODULEIMPLHOLDER_HPP_
#define PULSAR_GUARD_MODULEMANAGER__MODULEIMPLHOLDER_HPP_

#include "pulsar/python/Convert.hpp"

// Split into hpp and cpp files so we don't have to
// include ModuleBase.hpp here
namespace pulsar{
namespace modulebase {
class ModuleBase;
}
}



namespace pulsar{
namespace modulemanager {
namespace detail {


/*! \brief Storage of a module for use in ModulePtr
 *
 * Objects of this class (and derived classes)
 * are responsible for actual ownership
 * of the module data themselves, including deletion.
 */
class ModuleIMPLHolder
{
    public:
        ModuleIMPLHolder() = default;
        virtual ~ModuleIMPLHolder() = default;

        ModuleIMPLHolder(const ModuleIMPLHolder &) = delete;
        ModuleIMPLHolder & operator=(const ModuleIMPLHolder &) = delete;
        ModuleIMPLHolder(ModuleIMPLHolder &&) = delete;
        ModuleIMPLHolder & operator=(ModuleIMPLHolder &&) = delete;

        /*! \brief Return a C++ pointer to the module base interface
         *
         * Ownership remains the responsibility of thie IMPLHolder
         */
        virtual modulebase::ModuleBase * CppPtr(void) const = 0;


        /*! \brief Return a python object representing the module
         *
         * The python object will share ownership of the module
         * with this IMPL object.
         */
        virtual pybind11::object PythonObject(void) const = 0;


        /*! \brief Determine if the held module is a given type
         *
         * \tparam T Type of module to compare to. Should be a type that is
         *           derived from ModuleBase
         */
        template<typename T>
        bool IsType(void) const
        {
            using namespace pulsar::exception;

            if(CppPtr() == nullptr)
                throw GeneralException("Null pointer in ModuleIMPLHolder");

            T * ptr = dynamic_cast<T *>(CppPtr());

            return(ptr != nullptr);
        }

};




/*! \brief IMPL holder for C++ modules
 *
 * \tparam T The base module type (not the actual type)
 */
template<typename T>
class CppModuleIMPLHolder : public ModuleIMPLHolder
{
    public:
        /*! \brief Construct by reference to unique pointer
         *
         * Takes ownership of the data in the unique_ptr
         */
        CppModuleIMPLHolder(std::unique_ptr<T> && mod)
             : mod_(std::move(mod)) { }


        virtual modulebase::ModuleBase * CppPtr(void) const
        {
            using namespace pulsar::exception;
            if(!mod_)
                throw GeneralException("Null pointer in CppModuleIMPLHolder");
            return mod_.get();
        }


        virtual pybind11::object PythonObject(void) const
        {
            using namespace pulsar::exception;
            if(!mod_)
              throw GeneralException("Null pointer in CppModuleIMPLHolder");
            T * ptr = mod_.get();
            pybind11::object o = python::ConvertToPy(ptr, pybind11::return_value_policy::reference);

            if(!o)
                throw GeneralException("Null python object in CppModuleIMPLHolder");

            return o;
        }


    private:
        std::unique_ptr<T> mod_;
};




class PyModuleIMPLHolder : public ModuleIMPLHolder
{
    public:
        /*! \brief Construct by shallow copying a python object
         *
         * This IMPL object will share ownership with \p mod.
         */
        PyModuleIMPLHolder(const pybind11::object & mod);

        virtual modulebase::ModuleBase * CppPtr(void) const;

        virtual pybind11::object PythonObject(void) const;

    private:
        pybind11::object mod_;
};



} // close namespace detail
} // close namespace modulemanager
} // close namespace pulsar

#endif

