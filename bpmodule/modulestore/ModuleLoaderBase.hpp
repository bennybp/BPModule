/*! \file
 *
 * \brief Base class for loading and storing of modules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_MODULELOADERBASE_HPP_
#define _GUARD_MODULELOADERBASE_HPP_

#include "bpmodule/modulestore/ModuleStore.hpp"


namespace bpmodule {
namespace modulestore {

template<typename T>
class ModuleLoaderBase
{
    public:
        ModuleLoaderBase(ModuleStore * mst)
            : mst_(mst)
        { }


        ~ModuleLoaderBase()
        {
            //! \todo objects_ should definitely be empty now
        }


        ModuleLoaderBase(const ModuleLoaderBase & rhs)             = delete;
        ModuleLoaderBase(ModuleLoaderBase && rhs)                  = delete;
        ModuleLoaderBase & operator=(const ModuleLoaderBase & rhs) = delete;
        ModuleLoaderBase & operator=(ModuleLoaderBase && rhs)      = delete;
       
    protected: 

        /*! \brief Adds a module to the ModuleStore
         * 
         * \copydetails ModuleStore::AddModule
         *
         */
        void AddModule(const std::string & key,
                       ModuleStore::ModuleGeneratorFunc func,
                       ModuleStore::ModuleRemoverFunc dfunc,
                       const ModuleInfo & minfo)
        {
            mst_->AddModule(key, func, dfunc, minfo);
        }



        /*! \brief
         */
        void CopyObject(unsigned long id, T obj) // calls copy constructor
        {
            TakeObject(id, std::move(obj));
        }



        /*! \brief
         */
        void TakeObject(unsigned long id, T && obj)
        {
            objects_.emplace(id, std::move(obj));
        }



        /*! \brief
         */
        void DeleteObject(unsigned long id)
        {
            objects_.erase(id);
        }



        /*! \brief
         */
        void DeleteAll(void)
        {
            //! \todo check if empty
            objects_.clear();
        }


    private:
        typedef std::unordered_map<unsigned long, T> ObjectMap;

        ModuleStore * mst_;
        ObjectMap objects_;
};

} // close namespace modulestore
} // close namespace bpmodule


#endif
