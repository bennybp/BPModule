/*! \file
 *
 * \brief Loading and storing of C modules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_CMODULELOADER_HPP_
#define _GUARD_CMODULELOADER_HPP_

#include "bpmodule/modulestore/ModuleLoaderBase.hpp"


namespace bpmodule {
namespace modulestore {


/*! \brief Loads C/C++ modules
 *
 * This class is the ultimate owner of all created
 * C/C++ module objects, which it stores via ModuleLoaderBase (as smart pointers)
 */
class CModuleLoader : public ModuleLoaderBase< std::unique_ptr<modulebase::ModuleBase> >
{
    public:

        /*! \copydoc ModuleLoaderBase::ModuleLoaderBase
         */ 
        CModuleLoader(ModuleStore * mst);

        ~CModuleLoader();

        CModuleLoader(const CModuleLoader & rhs)             = delete;
        CModuleLoader(CModuleLoader && rhs)                  = delete;
        CModuleLoader & operator=(const CModuleLoader & rhs) = delete;
        CModuleLoader & operator=(CModuleLoader && rhs)      = delete;



        /*! \brief Loads an SO file for a C/C++ module
         *
         * The module path and name must be stored in the module info
         *
         * \throw bpmodule::exception::GeneralException if there is a problem loading
         *        the module (does not exist, etc)
         *
         * \exstrong
         *
         * \param [in] key The key for this module
         * \param [in] minfo The module information, including the path and name
         */ 
        void LoadSO(const std::string & key, const boost::python::dict & minfo);



    private:
        //! This object's base class
        typedef ModuleLoaderBase< std::unique_ptr<modulebase::ModuleBase> > BASE;

        //! Pointer to a creation function in an SO file
        typedef modulebase::ModuleBase *(*CreateFunc)(const std::string &, unsigned long, ModuleStore &, const ModuleInfo &);


        //! Stores handles to to open SO files
        std::unordered_map<std::string, void *> handles_;


        /*! \brief Wraps the function pointer in an SO file
         *
         * \exstrong
         *
         * \param [in] fn Creation function pointer in SO file
         * \param [in] key Key of the module to create
         * \param [in] id ID of the new module
         * \param [in] mstore ModuleStore that is in charge of this object
         * \param [in] minfo The information for this module (including options)
         * \return Pointer to a new object derived from ModuleBase
         */
        modulebase::ModuleBase * CreateWrapper_(CreateFunc fn,
                                                const std::string & key,
                                                unsigned long id,
                                                ModuleStore & mstore,
                                                const ModuleInfo & minfo);

        //! \copydoc LoadSO
        void LoadSO_(const std::string & key, const ModuleInfo & minfo);

};

} // close namespace modulestore
} // close namespace bpmodule


#endif
