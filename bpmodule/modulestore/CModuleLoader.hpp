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


class CModuleLoader : public ModuleLoaderBase< std::unique_ptr<modulebase::ModuleBase> >
{
    public:
        CModuleLoader(ModuleStore * mst);
        ~CModuleLoader();

        CModuleLoader(const CModuleLoader & rhs)             = delete;
        CModuleLoader(CModuleLoader && rhs)                  = delete;
        CModuleLoader & operator=(const CModuleLoader & rhs) = delete;
        CModuleLoader & operator=(CModuleLoader && rhs)      = delete;

        void LoadSO(const std::string & key, const boost::python::dict & minfo);

    private:
        typedef ModuleLoaderBase< std::unique_ptr<modulebase::ModuleBase> > BASE;
        typedef modulebase::ModuleBase *(*CreateFunc)(const std::string &, unsigned long, ModuleStore &, const ModuleInfo &);



        typedef std::unordered_map<std::string, void *> HandleMap;
        HandleMap handles_;


        modulebase::ModuleBase * CreateWrapper_(CreateFunc fn,
                                                const std::string & key,
                                                unsigned long id,
                                                ModuleStore & mstore,
                                                const ModuleInfo & minfo);

        void LoadSO_(const std::string & key, const ModuleInfo & minfo);

};

} // close namespace modulestore
} // close namespace bpmodule


#endif
