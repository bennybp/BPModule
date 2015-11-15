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


        /*! \brief Get all module information
         *
         * \throw std::logic_error if there is a severe developer error
         */ 
        modulelocator::ModuleInfo & MInfo(void);


        /*! \brief Get the data from the graph node
         *
         * \throw std::logic_error if there is a severe developer error
         */ 
        const modulelocator::ModuleInfo & MInfo(void) const;



        /*! \brief Print the information for this module
         *
         * \throw std::logic_error if there is a severe developer error
         */
        void Print(void) const; 

        ///@}


    protected:
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




    private:
        // allow ModuleLocator to set up the pointers
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
        /*! \brief Set the mlocator_ pointer
         */
        void SetMLocator_(modulelocator::ModuleLocator * mloc) noexcept;

        /*! \brief Set the graph data pointer
         */
        void SetGraphData_(modulelocator::GraphNodeData * gdat) noexcept;
};


} // close namespace modulebase
} // close namespace bpmodule

#endif
