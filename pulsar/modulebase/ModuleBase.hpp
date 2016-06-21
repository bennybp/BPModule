/*! \file
 *
 * \brief The base class for all modules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEBASE__MODULEBASE_HPP_
#define PULSAR_GUARD_MODULEBASE__MODULEBASE_HPP_

#include <string>

#include "pulsar/types.h"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/modulemanager/ModuleManager.hpp"
#include "pulsar/output/OutputStream.hpp"
#include "pulsar/output/TeeBufToString.hpp"
#include "pulsar/util/Format.hpp"
#include "pulsar/python/Call.hpp"
#include "pulsar/datastore/Wavefunction.hpp" // needed by basically all derived classes


// forward declarations
namespace pulsar{

namespace datastore {
struct Wavefunction;
}

namespace options {
class OptionMap;
}

} // close namespace pulsar


// end forward declarations


namespace pulsar{
namespace modulebase {



/*! \brief A base class for modules
 *
 * This class is in charge of doing some common things (ie, storing options). All
 * modules will ultimately derive from this.
 */
class ModuleBase
{
    public:

        /*! \brief Constructor
         */
        ModuleBase(ID_t id, const char * modtype);


        virtual ~ModuleBase();

        ModuleBase(const ModuleBase & /*rhs*/)              = delete;
        ModuleBase(ModuleBase && /*rhs*/)                   = delete;
        ModuleBase & operator= (const ModuleBase & /*rhs*/) = delete;
        ModuleBase & operator= (ModuleBase && /*rhs*/)      = delete;


        /*! \brief Get the unique ID of this module
         *
         * \exnothrow
         */
        ID_t id(void) const noexcept;


        /*! \brief Get the key of this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        std::string key(void) const;


        /*! \brief Get the descriptive name of this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        std::string name(void) const;


        /*! \brief Get the version of this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        std::string version(void) const;


        /*! \brief Get the module type
         *
         * ie, Test_Base, TwoElectronIntegral, etc
         */
        std::string module_type(void) const;


        /*! \brief Get the OptionMap object for this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        const datastore::OptionMap & options(void) const;


        /*! \brief Get the OptionMap object for this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        datastore::OptionMap & options(void);


        /*! \brief Print the information for this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        void print(std::ostream & os) const;


        /*! \brief Enable debug output for this module
         */
        void enable_debug(bool debug) noexcept;


        /*! \brief Is debug output enabled for this module
         */
        bool debug_enabled(void) const noexcept;



        /*! \brief Return a pointer to my node on the module tree
         *
         * \throw std::logic_error if there is a severe developer error
         */
        const modulemanager::ModuleTreeNode & my_node(void) const;


        /*! \brief Get the output from this module's tree node
         */
        std::string get_output(void) const;


        /*! \brief Create a module that is a child of this one
         */
        template<typename T>
        modulemanager::ModulePtr<T> create_child(const std::string & key) const
        {
            return mlocator_->get_module<T>(key, id_);
        }


        /*! \brief Create a module that is a child of this one,
         *         obtaining the modulekey from an option
         */
        template<typename T>
        modulemanager::ModulePtr<T> create_child_from_option(const std::string & optionkey) const
        {
            std::string modulekey = options().get<std::string>(optionkey);
            return mlocator_->get_module<T>(modulekey, id_);
        }



        /*! \brief Create a module that is a child of this one
         *
         * Python version
         */
        pybind11::object create_child_py(const std::string & key) const;

        /*! \brief Create a module that is a child of this one
         *         obtaining the modulekey from an option
         *
         * Python version
         */
        pybind11::object create_child_from_option_py(const std::string & optionkey) const;


    protected:
        /*! \brief Out tee string buffer
         */
        output::TeeBufToString tbts_;

        /*! \brief Our output stream
         */
        output::OutputStream out;


        /*! \brief Get the internal ModuleManager that is in charge of this module
         *
         * \throw std::logic_error if it hasn't been set
         */
        modulemanager::ModuleManager & module_manager(void) const;


        /*! \brief Get the cache object for this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        datastore::CacheData & cache(void) const noexcept;



        ////////////////////////////////////////////////////
        // WRAPPERS FOR DERIVED CLASS FUNCTION CALLS
        ////////////////////////////////////////////////////

        std::string exception_desc(void) const
        {
            return util::format_string("[%?] (%?) %? v%?", id(), key(), name(), version());
        }

        /*! \brief Quickly call a function, catching exceptions
         *
         * Meant to be used to call virtual functions that are
         * implemented in a the derived class.
         *
         * This version is "fast" - it performs minimal other steps
         * around the call to the other function.
         *
         * \tparam R Return type
         * \tparam P Derived class type
         * \tparam Targs1 Arguments types for the function
         * \tparam Targs2 Arguments types actually passed in
         *
         * \param [in] func Pointer to member function of class P and returning R
         * \param [in] args Arguments to forward
         */
        template<typename R, typename P, typename ... Targs1, typename ... Targs2>
        R fast_call_function( R(P::*func)(Targs1...), Targs2 &&... args)
        {
            //////////////////////////////////////////////////////////////////
            // So you think you like pointers and templates?
            //////////////////////////////////////////////////////////////////
            using exception::GeneralException;

            try {
                static_assert(std::is_base_of<ModuleBase, P>::value, "Cannot call function of unrelated class");

                P * ptr = dynamic_cast<P *>(this);                     // cast this to type P
                return ((*ptr).*func)(std::forward<Targs1>(args)...);  // call the function
            }
            catch(GeneralException & ex)
            {
                ex.append_info("from", exception_desc());
                throw;
            }
            catch(std::exception & ex)
            {
                throw GeneralException(ex, "what", ex.what(),
                                       "from", exception_desc());
            }
            catch(...)
            {
                throw GeneralException("Caught unknown exception. Get your debugger warmed up.",
                                       "from", exception_desc());
            }
        }

        /*! \copydocs fast_call_function */
        template<typename R, typename P, typename ... Targs1, typename ... Targs2>
        R fast_call_function( R(P::*func)(Targs1...) const, Targs2 &&... args) const
        {
            using exception::GeneralException;

            try {
                static_assert(std::is_base_of<ModuleBase, P>::value, "Cannot call function of unrelated class");

                const P * ptr = dynamic_cast<const P *>(this);         // cast this to type P
                return ((*ptr).*func)(std::forward<Targs1>(args)...);  // call the function
            }
            catch(GeneralException & ex)
            {
                ex.append_info("from", exception_desc());
                throw;
            }
            catch(std::exception & ex)
            {
                throw GeneralException(ex, "what", ex.what(),
                                       "from", exception_desc());
            }
            catch(...)
            {
                throw GeneralException("Caught unknown exception. Get your debugger warmed up.",
                                       "from", exception_desc());
            }
        }



        /*! \brief Call a function, catching exceptions
         *
         * Meant to be used to call virtual functions that are
         * implemented in a the derived class.
         *
         * \tparam R Return type
         * \tparam P Derived class type
         * \tparam Targs1 Arguments types for the function
         * \tparam Targs2 Arguments types actually passed in
         *
         * \param [in] func Pointer to member function of class P and returning R
         * \param [in] args Arguments to forward
         */
        template<typename R, typename P, typename ... Targs1, typename ... Targs2>
        R call_function( R(P::*func)(Targs1...), Targs2 &&... args)
        {
            return fast_call_function<R>(func, std::forward<Targs1>(args)...);
        }

        /*! \copydocs call_function */
        template<typename R, typename P, typename ... Targs1, typename ... Targs2>
        R call_function( R(P::*func)(Targs1...) const, Targs2 &&... args) const
        {
            return fast_call_function<R>(func, std::forward<Targs1>(args)...);
        }

        /*! \brief Calls a python function that overrides a virtual function */
        template<typename R, typename ... Targs>
        R call_py_override(const char * name, Targs &&... args)
        {
            // Module information is not appended to the exception since
            // this should always be called from the derived class
            pybind11::gil_scoped_acquire gil;
            pybind11::function overload = pybind11::get_overload(this, name);
            if(overload)
                return python::call_py_func<R>(overload, std::forward<Targs>(args)...);
            else
                throw exception::GeneralException("Cannot find overridden function", "vfunc", name);
        }

        /*! \brief Calls a python function that overrides a virtual function */
        template<typename R, typename ... Targs>
        R call_py_override(const char * name, Targs &&... args) const
        {
            // Module information is not appended to the exception since
            // this should always be called from the derived class
            pybind11::gil_scoped_acquire gil;
            pybind11::function overload = pybind11::get_overload(this, name);
            if(overload)
                return python::call_py_func<R>(overload, std::forward<Targs>(args)...);
            else
                throw exception::GeneralException("Cannot find overridden function", "vfunc", name);
        }


        /*! \brief See if this class has a member implemented in python */
        bool has_py_override(const char * name) const
        {
            pybind11::function overload = pybind11::get_overload(this, name);
            return (bool)overload;
        }


        ///////////////////////////////////////////
        // Helpers for derived classes
        ///////////////////////////////////////////

        /*! \brief Checks a python buffer for appropriate types and dimensions and returns
         *         the internal pointer
         *
         * \throw pulsar::exception::GeneralException if the type or number of dimensions
         *        doesn't match
         *
         * \tparam T The expected type stored
         * \param [in] ndim Expected number of dimensions
         * \return A pair with the first element being the pointer and the second being
         *         a vector of edge lengths
         */
        template<typename T>
        std::pair<T *, std::vector<size_t>>
        python_buffer_to_ptr(pybind11::buffer & buf, int ndim)
        {
            using exception::GeneralException;

            pybind11::buffer_info info = buf.request();
            if (info.format != pybind11::format_descriptor<T>::value)
                throw GeneralException("Bad format of python buffer",
                                       "format", info.format,
                                       "desired format", pybind11::format_descriptor<T>::value,
                                       "from", exception_desc());


            if (info.ndim != ndim)
                throw GeneralException("Bad number of dimensions for python buffer",
                                       "ndim", info.ndim,
                                       "desired ndim", ndim,
                                       "from", exception_desc());

            return { reinterpret_cast<T *>(info.ptr), info.shape };
        }


    private:
        // allow ModuleManager to set up the pointers
        friend class modulemanager::ModuleManager;

        //! The unique ID of this module
        const ID_t id_;

        //! The type of module this is
        const char * modtype_;

        //! The ModuleManager in charge of this module
        modulemanager::ModuleManager * mlocator_;

        //! My tree node
        modulemanager::ModuleTreeNode * treenode_;

        //! My cache
        datastore::CacheData * cache_;


        ////////////////////
        // Functions
        ////////////////////

        /*! \brief Set the mlocator_ pointer
         */
        void set_module_manager_(modulemanager::ModuleManager * mloc) noexcept;


        /*! \brief Set the tree node pointer
         */
        void set_tree_node_(modulemanager::ModuleTreeNode * node) noexcept;


        /*! \brief Get the tree node pointer
         *
         * non-const version is private
         */
        modulemanager::ModuleTreeNode & my_node(void);


        /*! \brief Set the cache object to use
         */
        void set_cache_(datastore::CacheData * cache) noexcept;
};


/*! \brief Forward some protected functions so that python
 *         can access them
 */
#define MODULEBASE_FORWARD_PROTECTED_TO_PY \
    using ModuleBase::cache; \
    using ModuleBase::module_manager; \
    using ModuleBase::out;


} // close namespace modulebase
} // close namespace pulsar

#endif
