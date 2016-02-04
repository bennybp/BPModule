/*! \file
 *
 * \brief Storage of module creation functions (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULEMANAGER__MODULECREATIONFUNCS_HPP_
#define BPMODULE_GUARD_MODULEMANAGER__MODULECREATIONFUNCS_HPP_

#include <functional>
#include <unordered_map>

#include "bpmodule/modulemanager/ModuleIMPLHolder.hpp"
#include "bpmodule/python/Call.hpp"


namespace bpmodule {
namespace modulemanager {

        
/*! \brief Stores creation functions for modules within a supermodule
 *
 * This gets returned from a loaded supermodule and contains a map
 * of module names to creation functions. These functions are then
 * used to create modules.
 */
class ModuleCreationFuncs
{
    public:
        //! Creation function for a single module. Only argument is its id
        typedef std::function<detail::ModuleIMPLHolder * (unsigned long)> Func;


        /////////////////////////////////////////////////////////////
        // All compiler-generated constructors and operator= are ok
        /////////////////////////////////////////////////////////////


        /*! \brief Add a creator for a C++ module
         * 
         * \tparam T The module to add. This is the full type, not the type
         *           of the base class.
         *
         * \param [in] modulename The name of the module
         */ 
        template<typename T>
        void AddCppCreator(const std::string & modulename)
        {
            Func cc = std::bind(&ModuleCreationFuncs::CppConstructorWrapper<T>, std::placeholders::_1);
            creators_.emplace(modulename, cc);
        }


        /*! \brief Add a creator for a python module
         * 
         * \param [in] modulename The name of the module
         * \param [in] cls The module to add (a python class).
         */ 
        void AddPyCreator(const std::string & modulename, const pybind11::object & cls)
        {
            Func m = std::bind(&ModuleCreationFuncs::PyConstructorWrapper, cls, std::placeholders::_1);
            creators_.emplace(modulename, m);
        }


        /*! \brief See if this object can create a module of the given name
         *
         * \param [in] modulename The name of the module to query
         * \return True if this object can create a module with name \p modulename
         */
        bool HasCreator(const std::string & modulename) const
        {
            return creators_.count(modulename);
        }


        /*! \brief Get the creator function for a module
         * 
         * \throw bpmodule::exception::GeneralException if there isn't a
         *        creator for that module name.
         *
         * \param [in] modulename The name of the module
         * \return A function that can create a module with the name \p modulename
         */
        const Func & GetCreator(const std::string & modulename) const
        {
            if(!HasCreator(modulename))
                throw exception::GeneralException("I don't have a creator for this module",
                                       "modulename", modulename);

            return creators_.at(modulename);
        }


        /*! \brief Delete all the stored creators
         */ 
        void Clear(void)
        {
            creators_.clear();
        }


    private:

        //! Storage of all the creation functions
        std::unordered_map<std::string, Func> creators_;


        /*! \brief Wrap construction of a C++ object
         */ 
        template<typename T>
        static
        detail::ModuleIMPLHolder *
        CppConstructorWrapper(unsigned long i)
        {
            std::unique_ptr<typename T::BaseType> p(new T(i));
            return new detail::CppModuleIMPLHolder<typename T::BaseType>(std::move(p));
        }


        /*! \brief Wrap construction of a python object (class)
         */ 
        static
        detail::ModuleIMPLHolder *
        PyConstructorWrapper(const pybind11::object & cls, unsigned long i)
        {
            pybind11::object o = python::CallPyFunc<pybind11::object>(cls, i);
            return new detail::PyModuleIMPLHolder(std::move(o));
        }
};


} // close namespace modulemanager
} // close namespace bpmodule


#endif
