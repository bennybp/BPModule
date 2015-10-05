/*! \file
 *
 * \brief Base class for loading and storing of modules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_MODULELOADERBASE_HPP_
#define _GUARD_MODULELOADERBASE_HPP_

#include "bpmodule/modulestore/ModuleStore.hpp"
#include "bpmodule/output/Output.hpp"


namespace bpmodule {
namespace modulestore {


/*! \brief A base class for module loaders
 *
 * \tparam T The type of object to store
 */
template<typename T>
class ModuleLoaderBase
{
    public:
        /*! \brief Constructor
         *
         * \param [in] mst ModuleStore to associate with this object
         */
        ModuleLoaderBase(ModuleStore * mst)
            : mst_(mst)
        { }


        virtual ~ModuleLoaderBase()
        {
            if(objects_.size())
                output::Error("ModuleLoaderBase: I still %1% have objects!!!", objects_.size());
        }


        ModuleLoaderBase(const ModuleLoaderBase & rhs)             = delete;
        ModuleLoaderBase(ModuleLoaderBase && rhs)                  = delete;
        ModuleLoaderBase & operator=(const ModuleLoaderBase & rhs) = delete;
        ModuleLoaderBase & operator=(ModuleLoaderBase && rhs)      = delete;

    protected:

        /*! \brief Inserts a module to the ModuleStore
         *
         * \copydetails ModuleStore::InsertModule
         *
         */
        void InsertModule(const std::string & key,
                       ModuleStore::ModuleGeneratorFunc func,
                       ModuleStore::ModuleRemoverFunc dfunc,
                       ModuleInfo & minfo)
        {
            mst_->InsertModule(key, func, dfunc, minfo);
        }



        /*! \brief Copies an instantiated object into internal storage
         *
         * Really meant for Python objects
         *
         * \exstrong
         *
         * \param [in] id Id of the new object
         * \param [in] obj Object to copy
         */
        void CopyObject(unsigned long id, T obj) // calls copy constructor
        {
            TakeObject(id, std::move(obj));
        }



        /*! \brief Moves an instantiated object into internal storage
         *
         * Really meant for std::unique_ptr objects
         *
         * \exstrong
         *
         * \param [in] id Id of the new object
         * \param [in] obj Object to move
         */
        void TakeObject(unsigned long id, T && obj)
        {
            objects_.emplace(id, std::move(obj));
        }



        /*! \brief Deletes an object from internal storage
         *
         * This destructs the object. It shouldn't be used after this
         *
         * \throwno Should only throw an exception if a destructor
         *          throws an exception, which shouldn't happen.
         *
         * \param [in] id The ID to delete
         */
        void DeleteObject(unsigned long id)
        {
            objects_.erase(id);
        }



        /*! \brief Deletes all objects in the internal storage
         *
         * All objects will be destructed and unusable afterwards
         *
         * \throwno Should only throw an exception if a destructor
         *          throws an exception, which shouldn't happen.
         */
        void DeleteAll(void)
        {
            objects_.clear();
        }


        /*! \brief Returns the number of stored objects
         *
         * \exnothrow
         *
         * \return Number of objects in the internal storage
         */
        size_t Size(void) const noexcept
        {
            return objects_.size();
        }


    private:
        //! Internal storage - maps ID to objects
        std::unordered_map<unsigned long, T> objects_;

        //! Pointer to the associated ModuleStore object
        ModuleStore * mst_;
};

} // close namespace modulestore
} // close namespace bpmodule


#endif
