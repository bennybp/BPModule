/*! \file
 *
 * \brief The base class for all modules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_MODULEBASE_HPP_
#define _GUARD_MODULEBASE_HPP_

#include "bpmodule/datastore/CalcData.hpp"

// forward declarations
namespace bpmodule {
namespace modulelocator {
class ModuleLocator;
struct ModuleInfo;
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
         *
         * \param [in] id The unique ID for this module
         * \param [in] mlocator The ModuleLocator in charge of this module
         * \param [in] minfo The information about this module
         */
        ModuleBase(unsigned long id,
                   modulelocator::ModuleLocator & mlocator,
                   modulelocator::ModuleInfo & minfo);

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


        /*! \brief Get the OptionMap object for this module
         *
         * \exnothrow
         */ 
        options::OptionMap & Options(void) noexcept;


        /*! \brief Get the OptionMap object for this module
         *
         * \exnothrow
         */ 
        const options::OptionMap & Options(void) const noexcept;


        /*! \brief Print the information for this module
         */
        void Print(void) const; 

        ///@}


    protected:
        /*! \brief Get the internal ModuleLocator that is in charge of this module
         */ 
        modulelocator::ModuleLocator & MLocator(void) noexcept;


    private:
        //! The unique ID of this module
        unsigned long id_;

        //! All the information for this module
        modulelocator::ModuleInfo & minfo_;

        //! The ModuleLocator in charge of this module
        modulelocator::ModuleLocator & mlocator_;
};


} // close namespace modulebase
} // close namespace bpmodule

#endif
