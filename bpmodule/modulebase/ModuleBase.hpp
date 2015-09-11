/*! \file
 *
 * \brief The base class for all modules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_MODULEBASE_HPP_
#define _GUARD_MODULEBASE_HPP_

#include "bpmodule/datastore/CalcData.hpp"
#include "bpmodule/datastore/OptionMap.hpp"

// forward declarations
namespace bpmodule {
namespace modulestore {
class ModuleStore;
class ModuleInfo;
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
         *
         * \param [in] id The unique ID for this module
         * \param [in] mstore The ModuleStore in charge of this module
         * \param [in] minfo The information about this module
         */
        ModuleBase(unsigned long id,
                   modulestore::ModuleStore & mstore,
                   const modulestore::ModuleInfo & minfo);

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
         * \exnothrow
         */   
        const std::string & Key(void) const noexcept;


        /*! \brief Get the descriptive name of this module
         *
         * \exnothrow
         */   
        const std::string & Name(void) const noexcept;


        /*! \brief Get the version of this module
         *
         * \exnothrow
         */   
        const std::string & Version(void) const noexcept;


        /*! \brief Get an option from this module
         *
         * \copydetails bpmodule::datastore::OptionMap::Get        
         */
        template<typename T>
        T GetOption(const std::string & key) const
        {
            try {
                return options_.Get<T>(key);
            }
            catch(exception::GeneralException & ex)
            {
                // rethrow with module info
                ThrowException(ex.what(), ex.GetInfo());
                return T(); // to make compilers happy
            }
        }


        /*! \brief Return true if the module has an option with the specified key
         *
         * \copydetails bpmodule::datastore::OptionMap::Has
         */
        bool HasOption(const std::string & key) const;



        /*! \brief Determine if this module's options has data of a specific type for a key
         *
         * \copydetails bpmodule::datastore::OptionMap::HasType
         */ 
        template<typename T>
        bool HasOptionType(const std::string & key) const
        {
            try {
                return options_.HasType<T>(key);
            }
            catch(exception::GeneralException & ex)
            {
                // rethrow with module info
                ThrowException(ex.what(), ex.GetInfo());
                return T(); // to make compilers happy
            }
        }


        
    protected:
        /*! \brief Throw an exception
         * 
         * Throwing via this function will populate the exception with
         * some information about this module.
         *
         * \param [in] exwhat A descriptive string describing the problem
         * \param [in] exinfo Additional information
         */
        void ThrowException(const std::string & exwhat,
                            const exception::GeneralException::ExceptionInfo & exinfo = {}) const;


        /*! \brief Get the internal ModuleStore that is in charge of this module
         */ 
        modulestore::ModuleStore & MStore(void) noexcept;


        /*! \brief Throw an exception (to be called from a python class derived from this)
         * 
         * \copydetails ThrowException(const std::string &, const exception::GeneralException::ExceptionInfo &) const
         *
         * The exinfo parameter is a list of tuples
         */ 
        void ThrowException(const std::string & exwhat,
                            const boost::python::list & exinfo);


    private:
        //! The unique ID of this module
        unsigned long id_;

        //! The key is was created under
        std::string key_;

        //! A descriptive name of the module
        std::string name_;

        //! The version of the module
        std::string version_;

        //! Options passed to this module
        datastore::OptionMap options_;

        //! The ModuleStore in charge of this module
        modulestore::ModuleStore & mstore_;
};


} // close namespace modulebase
} // close namespace bpmodule

#endif
