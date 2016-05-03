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
        ID_t ID(void) const noexcept;


        /*! \brief Get the key of this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        std::string Key(void) const;


        /*! \brief Get the descriptive name of this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        std::string Name(void) const;


        /*! \brief Get the version of this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        std::string Version(void) const;


        /*! \brief Get the module type
         * 
         * ie, Test_Base, TwoElectronIntegral, etc
         */
        std::string ModuleType(void) const;


        /*! \brief Get the OptionMap object for this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        const datastore::OptionMap & Options(void) const;

        /*! \brief Print the information for this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        void Print(std::ostream & os) const;


        /*! \brief Enable debug output for this module
         */
        void EnableDebug(bool debug) noexcept; 


        /*! \brief Is debug output enabled for this module
         */
        bool DebugEnabled(void) const noexcept; 



        /*! \brief Return a pointer to my node on the module tree
         *
         * \throw std::logic_error if there is a severe developer error
         */
        const modulemanager::ModuleTreeNode & MyNode(void) const;


        /*! \brief Get the initial wavefunction from this module's tree node
         *
         * \throw std::logic_error if there is a severe developer error
         */
        datastore::Wavefunction & InitialWfn(void);


        /*! \brief Get the initial wavefunction from this module's tree node
         *
         * \throw std::logic_error if there is a severe developer error
         */
        const datastore::Wavefunction & InitialWfn(void) const;
       
 
        /*! \brief Allows you to set the initial wavefunction on this module's tree node
         * 
         * \note Only really here to allow setting via python
         */
        void SetInitialWfn(const datastore::Wavefunction& Wfn);


        /*! \brief Get the final wavefunction from this module's tree node
         *
         * \throw std::logic_error if there is a severe developer error
         */
        datastore::Wavefunction & FinalWfn(void);


        /*! \brief Get the final wavefunction from this module's tree node
         *
         * \throw std::logic_error if there is a severe developer error
         */
        const datastore::Wavefunction & FinalWfn(void) const;
       
 
        /*! \brief Allows you to set the final wavefunction on this module's tree node
         * 
         * \note Only really here to allow setting via python
         */
        void SetFinalWfn(const datastore::Wavefunction& Wfn);


        /*! \brief Get the output from this module's tree node
         */
        std::string GetOutput(void) const; 


        /*! \brief Create a module that is a child of this one
         */ 
        template<typename T>
        modulemanager::ModulePtr<T> CreateChildModule(const std::string & key) const
        {
            return mlocator_->GetModule<T>(key, id_);
        }


        /*! \brief Create a module that is a child of this one,
         *         obtaining the modulekey from an option
         */ 
        template<typename T>
        modulemanager::ModulePtr<T> CreateChildModuleFromOption(const std::string & optionkey) const
        {
            std::string modulekey = Options().Get<std::string>(optionkey);
            return mlocator_->GetModule<T>(modulekey, id_);
        }



        /*! \brief Create a module that is a child of this one
         *
         * Python version 
         */ 
        pybind11::object CreateChildModulePy(const std::string & key) const;

        /*! \brief Create a module that is a child of this one
         *         obtaining the modulekey from an option
         *
         * Python version 
         */ 
        pybind11::object CreateChildModuleFromOptionPy(const std::string & optionkey) const;


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
        modulemanager::ModuleManager & MManager(void) const;



        /*! \brief Get the cache object for this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        datastore::CacheData & Cache(void) const noexcept;



        ////////////////////////////////////////////////////
        // WRAPPERS FOR DERIVED CLASS FUNCTION CALLS
        ////////////////////////////////////////////////////

        std::string ExceptionDescString(void) const
        {
            return util::FormatString("[%?] (%?) %? v%?", ID(), Key(), Name(), Version());
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
        R FastCallFunction( R(P::*func)(Targs1...), Targs2 &&... args)
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
                ex.AppendInfo("from", ExceptionDescString());
                throw;
            }
            catch(std::exception & ex)
            {
                throw GeneralException(ex, "what", ex.what(),
                                       "from", ExceptionDescString());
            }
            catch(...)
            {
                throw GeneralException("Caught unknown exception. Get your debugger warmed up.",
                                       "from", ExceptionDescString());
            }
        }

        /*! \copydocs FastCallFunction */
        template<typename R, typename P, typename ... Targs1, typename ... Targs2>
        R FastCallFunction( R(P::*func)(Targs1...) const, Targs2 &&... args) const
        {
            using exception::GeneralException;

            try {
                static_assert(std::is_base_of<ModuleBase, P>::value, "Cannot call function of unrelated class");

                const P * ptr = dynamic_cast<const P *>(this);         // cast this to type P
                return ((*ptr).*func)(std::forward<Targs1>(args)...);  // call the function
            }
            catch(GeneralException & ex)
            {
                ex.AppendInfo("from", ExceptionDescString());
                throw;
            }
            catch(std::exception & ex)
            {
                throw GeneralException(ex, "what", ex.what(),
                                       "from", ExceptionDescString());
            }
            catch(...)
            {
                throw GeneralException("Caught unknown exception. Get your debugger warmed up.",
                                       "from", ExceptionDescString());
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
        R CallFunction( R(P::*func)(Targs1...), Targs2 &&... args)
        {
            return FastCallFunction<R>(func, std::forward<Targs1>(args)...);
        }

        /*! \copydocs CallFunction */
        template<typename R, typename P, typename ... Targs1, typename ... Targs2>
        R CallFunction( R(P::*func)(Targs1...) const, Targs2 &&... args) const
        {
            return FastCallFunction<R>(func, std::forward<Targs1>(args)...);
        }

        /*! \brief Calls a python function that overrides a virtual function */
        template<typename R, typename ... Targs>
        R CallPyOverride(const char * name, Targs &&... args)
        {
            // Module information is not appended to the exception since
            // this should always be called from the derived class
            pybind11::gil_scoped_acquire gil;
            pybind11::function overload = pybind11::get_overload(this, name);
            if(overload)
                return python::CallPyFunc<R>(overload, std::forward<Targs>(args)...);
            else
                throw exception::GeneralException("Cannot find overridden function", "vfunc", name);
        }

        /*! \brief Calls a python function that overrides a virtual function */
        template<typename R, typename ... Targs>
        R CallPyOverride(const char * name, Targs &&... args) const
        {
            // Module information is not appended to the exception since
            // this should always be called from the derived class
            pybind11::gil_scoped_acquire gil;
            pybind11::function overload = pybind11::get_overload(this, name);
            if(overload)
                return python::CallPyFunc<R>(overload, std::forward<Targs>(args)...);
            else
                throw exception::GeneralException("Cannot find overridden function", "vfunc", name);
        }


        /*! \brief See if this class has a member implemented in python */
        bool HasPyOverride(const char * name) const
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
        PythonBufferToPtr(pybind11::buffer & buf, int ndim)
        {
            using exception::GeneralException;

            pybind11::buffer_info info = buf.request();
            if (info.format != pybind11::format_descriptor<T>::value())
                throw GeneralException("Bad format of python buffer",
                                       "format", info.format,
                                       "desired format", pybind11::format_descriptor<T>::value(),
                                       "from", ExceptionDescString());


            if (info.ndim != ndim)
                throw GeneralException("Bad number of dimensions for python buffer",
                                       "ndim", info.ndim,
                                       "desired ndim", ndim,
                                       "from", ExceptionDescString());

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
        void SetMManager_(modulemanager::ModuleManager * mloc) noexcept;


        /*! \brief Set the tree node pointer
         */
        void SetTreeNode_(modulemanager::ModuleTreeNode * node) noexcept;


        /*! \brief Get the tree node pointer
         * 
         * non-const version is private
         */
        modulemanager::ModuleTreeNode & MyNode(void);


        /*! \brief Set the cache object to use
         */
        void SetCache_(datastore::CacheData * cache) noexcept;
};


/*! \brief Forward some protected functions so that python
 *         can access them
 */
#define MODULEBASE_FORWARD_PROTECTED_TO_PY \
    using ModuleBase::Cache; \
    using ModuleBase::MManager; \
    using ModuleBase::out;


} // close namespace modulebase
} // close namespace pulsar

#endif
