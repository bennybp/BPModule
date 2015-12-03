/*! \file
 *
 * \brief The base class for all modules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_MODULEBASE_HPP_
#define _GUARD_MODULEBASE_HPP_

#include <string>
#include <boost/python/call_method.hpp>
#include <boost/python/object.hpp> //! \todo forward declare PyObject?

#include "bpmodule/exception/GeneralException.hpp"
#include "bpmodule/python_helper/BoostPython_fwd.hpp"


// forward declarations
namespace bpmodule {
namespace modulelocator {
class ModuleLocator;
struct ModuleInfo;
struct GraphNodeData;
}

namespace options {
class OptionMap;
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
class ModuleBase
{
    public:

        /*! \brief Constructor
         */
        ModuleBase(unsigned long id);

        /*! \brief Constructor for python
         */
        ModuleBase(PyObject * self, unsigned long id);


        virtual ~ModuleBase();

        ModuleBase(const ModuleBase & rhs)              = delete;
        ModuleBase(ModuleBase && rhs)                   = delete;
        ModuleBase & operator= (const ModuleBase & rhs) = delete;
        ModuleBase & operator= (ModuleBase && rhs)      = delete;


        //! \name Basic Info
        ///@ {

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


        /*! \brief Get the OptionMap object for this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        options::OptionMap & Options(void);


        /*! \brief Get the OptionMap object for this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        const options::OptionMap & Options(void) const;


        /*! \brief Get the internal ModuleLocator that is in charge of this module
         *
         * \throw std::logic_error if it hasn't been set
         */
        modulelocator::ModuleLocator & MLocator(void);


        /*! \brief Get the internal ModuleLocator that is in charge of this module
         *
         * \throw std::logic_error if it hasn't been set
         */
        const modulelocator::ModuleLocator & MLocator(void) const;


        /*! \brief Print the information for this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        void Print(void) const;


        /*! \brief See if this module is a python module
         */
        bool IsPythonModule(void) const noexcept;

        ///@}





    protected:

        /*! \brief Get all module information
         *
         * \throw std::logic_error if it hasn't been set
         */
        modulelocator::GraphNodeData & GraphData(void);


        /*! \brief Get all module information
         *
         * \throw std::logic_error if it hasn't been set
         */
        const modulelocator::GraphNodeData & GraphData(void) const;



        /*! \brief Throw an exception
         *
         * Will create the exception of the specified type, add the arguments,
         * and then add information about the module
         */
        template<typename T, typename ... Targs>
        void Throw(const Targs &... args)
        {
            T ex(args...);
            ex.AppendInfo("modulekey", Key());
            ex.AppendInfo("modulename", Name());
            ex.AppendInfo("moduleversion", Version());
            throw ex;
        }


        /*! \brief Call a python method
         */
        template<typename R, typename ... An>
        R CallPyMethod(const char * fname, const An &... args)
        {
            if (!IsPythonModule())
                Throw<exception::GeneralException>("Attempting to call a virtual function in a C++ module that is missing from the derived class");
            return boost::python::call_method<R>(pyself_, fname, args...);
        }



    private:
        // python self pointer
        // Only used if this is a python module. Otherwise it is null
        PyObject * pyself_;

        // allow ModuleLocator to set up the pointers
        // and to call MoveToPyObject_
        friend class modulelocator::ModuleLocator;



        //! The unique ID of this module
        const unsigned long id_;

        //! The ModuleLocator in charge of this module
        modulelocator::ModuleLocator * mlocator_;

        //! My graph node data
        modulelocator::GraphNodeData * graphdata_;



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
        void SetGraphData_(modulelocator::GraphNodeData * gdat) noexcept;


        /*! \brief Create a boost::python::object that manages this
         *
         * This object will now be managed by the python object.
         *
         * \warning Internal use only. Really should only be accessed via the ModuleLocator. 
         *
         * \warning Do not call more than once on an object, and do not call on
         *          one not created via new. The python object will
         *          manage this as if it were a smart pointer.
         *
         * \todo exception documentation/wrapping
         */
        virtual boost::python::object MoveToPyObject_(std::function<void(modulebase::ModuleBase *)> deleter) = 0;
};


} // close namespace modulebase
} // close namespace bpmodule

#endif
