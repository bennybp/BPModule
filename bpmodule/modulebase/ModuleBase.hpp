/*! \file
 *
 * \brief The base class for all modules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULEBASE__MODULEBASE_HPP_
#define BPMODULE_GUARD_MODULEBASE__MODULEBASE_HPP_

#include <string>

#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/modulelocator/ModuleLocator.hpp"
#include "bpmodule/util/FormatString.hpp"
#include "bpmodule/python/Call.hpp"


// forward declarations
namespace bpmodule {

namespace datastore {
struct GraphNodeData;
struct GraphNode;
struct Wavefunction;
}

namespace options {
class OptionMap;
}

// for friend
namespace modulebase {
namespace export_python {
extern "C" {
PyObject * PyInit_modulebase(void);
}
}
}

}
// end forward declarations


namespace bpmodule {
namespace modulebase {


/*! \brief A base class for modules
 *
 * This class is in charge of doing some common things (ie, storing options). All
 * modules will ultimately derive from this.
 */
class ModuleBase : public std::enable_shared_from_this<ModuleBase>
{
    public:

        /*! \brief Constructor
         */
        ModuleBase(unsigned long id, const char * modtype);


        virtual ~ModuleBase();

        ModuleBase(const ModuleBase & rhs)              = delete;
        ModuleBase(ModuleBase && rhs)                   = delete;
        ModuleBase & operator= (const ModuleBase & rhs) = delete;
        ModuleBase & operator= (ModuleBase && rhs)      = delete;


        /*! \brief Get the unique ID of this module
         *
         * \exnothrow
         */
        unsigned long ID(void) const noexcept;


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
         * ie, Test_Base, TwoElectronIntegralIMPL, etc
         */
        std::string ModuleType(void) const;



        /*! \brief Get the OptionMap object for this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        datastore::OptionMap & Options(void);


        /*! \brief Get the OptionMap object for this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        const datastore::OptionMap & Options(void) const;

        /*! \brief Print the information for this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        void Print(void) const;


        /*! \brief Return a pointer to my node on the graph
         *
         * \throw std::logic_error if there is a severe developer error
         */
        const datastore::GraphNode * MyNode(void) const;


        /*! \brief Get the wavefunction for this graph node
         *
         * \throw std::logic_error if there is a severe developer error
         */
        datastore::Wavefunction & Wfn(void);



    protected:
        /* This is the function created by BOOST_PYTHON_MODULE
         * This is needed to allow protected members to be
         * accessed to derived classes written in python
         * (alternative is a wrapper class, which is messy in our case)
         *
         * //! \todo we now have wrapper classes, but we would have to forward for all classes
         */
        friend PyObject * export_python::PyInit_modulebase(void);


        /*! \brief Get the internal ModuleLocator that is in charge of this module
         *
         * \throw std::logic_error if it hasn't been set
         */
        modulelocator::ModuleLocator & MLocator(void) const;



        /*! \brief Get the cache object for this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        datastore::CacheData & Cache(void) const noexcept;



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
            //////////////////////////////////////////////////////////////////
            // So you think you like pointers and templates?
            //////////////////////////////////////////////////////////////////
            try {
                static_assert(std::is_base_of<ModuleBase, P>::value, "Cannot call function of unrelated class");

                P * ptr = dynamic_cast<P *>(this);                     // cast this to type P
                return ((*ptr).*func)(std::forward<Targs1>(args)...);  // call the function
            }
            catch(exception::GeneralException & ex)
            {
                std::string s = util::FormatString("[%1%] (%2%) %3% v%4%", ID(), Key(), Name(), Version());
                ex.AppendInfo("from", s);
                throw;
            }
            catch(std::exception & ex)
            {
                std::string s = util::FormatString("[%1%] (%2%) %3% v%4%", ID(), Key(), Name(), Version());
                throw exception::GeneralException(ex, "what", ex.what(),
                                                  "from", s);
            }
            catch(...)
            {
                std::string s = util::FormatString("[%1%] (%2%) %3% v%4%", ID(), Key(), Name(), Version());
                throw exception::GeneralException("Caught unknown exception. Get your debugger warmed up.",
                                                  "from", s);
            }
        }



        /*! \brief Calls a python function that overrides a virtual function
         */ 
        template<typename R, typename ... Targs>
        R CallPyOverride(const char * name, Targs &&... args)
        {
            pybind11::gil_scoped_acquire gil;
            pybind11::function overload = pybind11::get_overload(this, name);
            if(overload)
            {
                try {
                    return python::CallPyFunc<R>(overload, std::forward<Targs>(args)...);
                }
                catch(exception::PythonCallException & ex)
                {
                    //ex.AppendInfo("vfunc", name);
                    throw;
                }
            }
            else
                throw exception::GeneralException("Cannot find overridden function", "vfunc", name);
        }



        /*! \brief Get the wavefunction for this graph node
         *
         * \throw std::logic_error if there is a severe developer error
         */
        const datastore::Wavefunction & Wfn(void) const;


        /*! \brief Create a module that is a child of this one
         */ 
        template<typename T>
        modulelocator::ModulePtr<T> CreateChildModule(const std::string & key) const
        {
            return mlocator_->GetModule<T>(key, id_);
        }



        /*! \brief Create a module that is a child of this one
         *
         * Python version 
         */ 
        pybind11::object CreateChildModulePy(const std::string & key) const;




    private:
        // allow ModuleLocator to set up the pointers
        friend class modulelocator::ModuleLocator;



        //! The unique ID of this module
        const unsigned long id_;


        //! The type of module this is
        const char * modtype_;


        //! The ModuleLocator in charge of this module
        modulelocator::ModuleLocator * mlocator_;

        //! My graph node
        datastore::GraphNode * graphnode_;

        //! My cache
        datastore::CacheData * cache_;


        ////////////////////
        // Functions
        ////////////////////
        //
        /*! \brief Get all module information from the graph node
         *
         * \throw std::logic_error if there is a severe developer error
         */
        modulelocator::ModuleInfo & MInfo_(void);


        /*! \brief Get all module information from the graph node
         *
         * \throw std::logic_error if there is a severe developer error
         */
        const modulelocator::ModuleInfo & MInfo_(void) const;


        /*! \brief Set the mlocator_ pointer
         */
        void SetMLocator_(modulelocator::ModuleLocator * mloc) noexcept;


        /*! \brief Set the graph data pointer
         */
        void SetGraphNode_(datastore::GraphNode * node) noexcept;


        /*! \brief Set the cache object to use
         */
        void SetCache_(datastore::CacheData * cache) noexcept;


        /*! \brief Get all module information stored on the graph
         *
         * \throw std::logic_error if it hasn't been set
         */
        datastore::GraphNodeData & GraphData(void);


        /*! \brief Get all module information stored on the graph
         *
         * \throw std::logic_error if it hasn't been set
         */
        const datastore::GraphNodeData & GraphData(void) const;

};


} // close namespace modulebase
} // close namespace bpmodule

#endif
