/*! \file
 *
 * \brief Storage of module creation functions (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEMANAGER__MODULECREATIONFUNCS_HPP_
#define PULSAR_GUARD_MODULEMANAGER__MODULECREATIONFUNCS_HPP_

#include <functional>
#include <map>

#include "pulsar/types.h"
#include "pulsar/modulemanager/ModuleIMPLHolder.hpp"
#include "pulsar/util/PythonHelper.hpp"


namespace pulsar{

/*! \brief Stores creation functions for modules within a supermodule
 *
 * This object contains a map of module names to functions that create
 * an object by that name. When a module is loaded, a
 * ModuleCreationFuncs object gets returned from the insert_supermodule
 * function. The ModuleManager then uses the information stored within
 * to populate its module creation map.
 */
class ModuleCreationFuncs
{
    public:
        //! Creation function for a single module
        typedef std::function<detail::ModuleIMPLHolder * (ID_t)> Func;


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
        void add_cpp_creator(const std::string & modulename)
        {
            Func cc = std::bind(&ModuleCreationFuncs::cpp_constructor_wrapper_<T>, std::placeholders::_1);
            creators_.emplace(modulename, cc);
        }


        /*! \brief Add a creator for a python module
         *
         * \param [in] modulename The name of the module
         * \param [in] cls The module to add. This should be a python class.
         */
        void add_py_creator(const std::string & modulename, const pybind11::object & cls)
        {
            //! \todo check if it is a class?
            Func m = std::bind(&ModuleCreationFuncs::py_constructor_wrapper_, cls, std::placeholders::_1);
            creators_.emplace(modulename, m);
        }


        /*! \brief Check to see if this object can create a module of the given name
         *
         * \param [in] modulename The name of the module to query
         * \return True if this object can create a module with name \p modulename
         */
        bool has_creator(const std::string & modulename) const
        {
            return creators_.count(modulename);
        }


        /*! \brief Get the creator function for a module
         *
         * \throw pulsar::PulsarException if there isn't a
         *        creator for that module name.
         *
         * \param [in] modulename The name of the module
         * \return A function that can create a module with the name \p modulename
         */
        const Func & get_creator(const std::string & modulename) const
        {
            if(!has_creator(modulename))
                throw PulsarException("I don't have a creator for this module",
                                       "modulename", modulename);

            return creators_.at(modulename);
        }


        /*! \brief Delete all the stored creators
         */
        void clear(void)
        {
            creators_.clear();
        }


    private:

        //! Storage of all the creation functions
        std::map<std::string, Func> creators_;


        /*! \brief Wrap construction of a C++ object
         *
         * \tparam T Full type of the module (not the base type)
         * \param [in] id The ID of the newly-created module
         */
        template<typename T>
        static
        detail::ModuleIMPLHolder *
        cpp_constructor_wrapper_(ID_t id)
        {
            // T::BaseType is the module base type (ie, TwoElectronIntegral, etc)
            // defined in the base type class.
            std::unique_ptr<typename T::BaseType> p(new T(id));
            return new detail::CppModuleIMPLHolder<typename T::BaseType>(std::move(p));
        }


        /*! \brief Wrap construction of a python object (class)
         *
         * \param [in] cls Python class
         * \param [in] id The ID of the newly-created module
         */
        static
        detail::ModuleIMPLHolder *
        py_constructor_wrapper_(const pybind11::object & cls, ID_t id)
        {
            pybind11::object o = call_py_func<pybind11::object>(cls, id);
            return new detail::PyModuleIMPLHolder(std::move(o));
        }
};

} // close namespace pulsar


#endif
