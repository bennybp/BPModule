#ifndef BPMODULE_GUARD_MODULEMANAGER__MODULECREATIONFUNCS_HPP_
#define BPMODULE_GUARD_MODULEMANAGER__MODULECREATIONFUNCS_HPP_

#include <functional>
#include <map>

#include "bpmodule/modulemanager/ModuleIMPLHolder.hpp"


namespace bpmodule {
namespace modulemanager {

        
/*! \brief Stores creation functions for modules
 *
 * This gets returned from the modules and contains a map
 * of modulename to creation functions.
 */
class ModuleCreationFuncs
{
    public:
        //! Creation function for a single module. Only argument is its id
        typedef std::function<detail::ModuleIMPLHolder * (unsigned long)> Func;

        // all compiler-generated constructors & assignment are ok


        /*! \brief Add a creator for a C++ module
         * 
         * \tparam T The module to add. This is the full type, not the type
         *           of the base class.
         */ 
        template<typename T>
        void AddCppCreator(const std::string & modulename)
        {
            Func cc = std::bind(&ModuleCreationFuncs::CppConstructorWrapper<T>, std::placeholders::_1);
            creators_.emplace(modulename, cc);
        }


        /*! \brief Add a creator for a python module
         */ 
        void AddPyCreator(const std::string & modulename, const pybind11::object & cls)
        {
            Func m = std::bind(&ModuleCreationFuncs::PyConstructorWrapper, cls, std::placeholders::_1);
            creators_.emplace(modulename, m);
        }


        bool HasCreator(const std::string & modulename) const
        {
            return creators_.count(modulename);
        }


        Func GetCreator(const std::string & modulename) const
        {
            if(!HasCreator(modulename))
                throw exception::GeneralException("I don't have a creator for this module",
                                       "modulename", modulename);

            return creators_.at(modulename);
        }

    private:

        std::map<std::string, Func> creators_;


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


        /*! \brief Wrap construction of a python object
         */ 
        static
        detail::ModuleIMPLHolder *
        PyConstructorWrapper(const pybind11::object & cls, unsigned long i)
        {
            pybind11::object o = cls.call(i);
            detail::ModuleIMPLHolder * ret = new detail::PyModuleIMPLHolder(o);
            return ret;
        }
};


} // close namespace modulemanager
} // close namespace bpmodule


#endif
