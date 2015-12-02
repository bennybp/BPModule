/*! \file
 *
 * \brief Main module database class (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_MODULELOCATOR_HPP_
#define _GUARD_MODULELOCATOR_HPP_

#include <unordered_map>
#include <atomic>

#include "bpmodule/modulelocator/Graph.hpp"
#include "bpmodule/modulelocator/ScopedModule.hpp"
#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/exception/ModuleCreateException.hpp"
#include "bpmodule/exception/ModuleLocatorException.hpp"


// forward declarations
namespace bpmodule {

namespace modulelocator {

template<typename T>
class ModuleLoaderBase;

class CModuleLoader;
class PyModuleLoader;
}
}
// end forward declarations

namespace bpmodule {
namespace modulelocator {



/*! \brief Module database
 *
 * Holds loaded modules for later use
 */
class ModuleLocator
{
    public:
        ModuleLocator();
        ~ModuleLocator();


        // no copy construction or assignment
        ModuleLocator(const ModuleLocator & rhs)             = delete;
        ModuleLocator(ModuleLocator && rhs)                  = delete;
        ModuleLocator & operator=(const ModuleLocator & rhs) = delete;
        ModuleLocator & operator=(ModuleLocator && rhs)      = delete;


        /*! \brief Returns the number of modules in the database
         *
         * \exnothrow
         */
        size_t Size(void) const noexcept;


        /*! \brief Returns the keys for all modules in the database
         */
        std::vector<std::string> GetKeys(void) const;


        /*! \brief Returns the information about a module with a given key
         *
         * \throw bpmodule::exception::ModuleLocatorException
         *        if the key doesn't exist in the database
         *
         * \param [in] key A module key
         */
        ModuleInfo KeyInfo(const std::string & key) const;


        
        /*! \brief Prints all the information about the loaded modules
         */
        void PrintInfo(void) const;


        /*! \brief Returns true if a module with the given key exists in the database
         *
         * \param [in] key A module key
         * \return True if the key exists in the map, false if it doesn't
         */
        bool Has(const std::string & key) const;



        /*! \brief Set the options for a module (python version)
         *
         * The dictionary has strings for keys and arbitrary data types for
         * the values
         *
         * \throw bpmodule::exception::ModuleLocatorException
         *        if the key doesn't exist in the database
         *
         * \throw bpmodule::exception::OptionException
         *        if there is a problem converting the options
         *        or if it is invalid
         *
         * \exstrong
         *
         *
         * \param [in] key A module key
         * \param [in] opt Options to set
         */
        void SetOptions(const std::string & key, const boost::python::dict & opt);




        /*! \brief Test the creation of all modules
         *
         * This goes through and tests the module creation for all keys. It does
         * not attempt to cast them, though. This is a simple sanity check
         *
         * \throw bpmodule::exception::ModuleCreateException if there is a problem
         */
        void TestAll(void);



        /*! \brief Return a new module object wrapped in an RAII-style scoping object
         *
         * \throw bpmodule::exception::ModuleLocatorException
         *        if the key doesn't exist in the database
         *
         * \throw bpmodule::exception::ModuleCreateException if there are other
         *        problems creating the module
         *
         * \exbasic
         *
         * \param [in] key A module key
         *
         * \return A ScopedModule for an object of the requested type
         *
         * \todo Move most of this to a source file
         */
        template<typename T>
        ScopedModule<T> GetModule(const std::string & key)
        {
            // may throw
            // CreateModule returns a pointer/deleter pair
            auto mod = CreateModule(key);

            T * dptr = dynamic_cast<T *>(mod.first);
            if(dptr == nullptr)
            {
                const ModuleInfo & mi = mod.first->MInfo();

                throw exception::ModuleCreateException("Bad cast for module", mi.path,
                                                       key, mi.name, 
                                                       "fromtype", mangle::DemangleCppType(mod.first),
                                                       "totype", mangle::DemangleCppType<T *>());
            }

            ScopedModule<T> ret(dptr, mod.second); // construction shouldn't throw?

            return ret;
        }



        /*! \brief Retrieve a module as a python object
         * 
         * \throw bpmodule::exception::ModuleLocatorException
         *        if the key doesn't exist in the database
         *
         * \throw bpmodule::exception::ModuleCreateException if there are other
         *        problems creating the module
         */
        boost::python::object GetModulePy(const std::string & key);



    protected:
        template<typename T>
        friend class ModuleLoaderBase;

        friend class CModuleLoader;
        friend class PyModuleLoader;

        //! A function that generates a module derived from ModuleBase
        typedef std::function<modulebase::ModuleBase *(const std::string &, unsigned long)> ModuleGeneratorFunc;


        //! A function that deletes a module (by id)
        typedef std::function<void(unsigned long)> ModuleRemoverFunc;



        /*! \brief Adds/inserts a module creator to the database
         *
         * \throw bpmodule::exception::ModuleLoaderException if the key
         *        already exists in the database
         *
         * \exstrong
         *
         * \param [in] key A module key
         * \param [in] func A function that generates the module
         * \param [in] dfunc A function that deletes the module
         * \param [in] minfo Information about the module
         */
        void InsertModule(const std::string & key, ModuleGeneratorFunc func,
                          ModuleRemoverFunc dfunc, const ModuleInfo & minfo);


    private:

        /*! \brief An entry for a module in the database
         */
        struct StoreEntry
        {
            ModuleInfo mi;             //!< Information for this module
            ModuleGeneratorFunc func;  //!< Function that creates a class from this module
            ModuleRemoverFunc dfunc;   //!< Function that deletes a class from this module
        };


        /*! \brief Actual storage object - maps keys to creation functions
         */
        std::unordered_map<std::string, StoreEntry> store_;


        /*! \brief Map for storing object removal information
         */
        std::unordered_map<unsigned long, ModuleRemoverFunc> removemap_;


        /*! \brief Map for storing created module information
         *
         * \todo will be replaced by a graph or tree
         */
        std::unordered_map<unsigned long, GraphNodeData> graphdata_;


        //! The id to assign to the next created module
        std::atomic<unsigned long> curid_;


        /*! \brief Map of cache data
         *
         * The key is a combination of the module name and version
         *
         * \todo is this enough to guarantee uniqueness?
         */
        std::unordered_map<std::string, datastore::CalcData> cachemap_;


        /*! \brief Obtain a module or throw exception
         *
         * \throw bpmodule::exception::ModuleLocatorException
         *        if the key doesn't exist
         *
         * \param [in] key A module key
         */
        const StoreEntry & GetOrThrow_(const std::string & key) const;



        /*! \brief Obtain a module or throw exception
         *
         * \throw bpmodule::exception::ModuleLocatorException
         *        if the key doesn't exist
         *
         * \param [in] key A module key
         */
        StoreEntry & GetOrThrow_(const std::string & key);


        /*! \brief Create a module and its deleter functor
         * 
         * \throw bpmodule::exception::ModuleLocatorException
         *        if the key doesn't exist in the database
         *
         * \throw bpmodule::exception::ModuleCreateException if there are other
         *        problems creating the module
         *
         */
        std::pair<modulebase::ModuleBase *, std::function<void(modulebase::ModuleBase *)>>
        CreateModule(const std::string & key);


        /*! \brief Removes a created module object from storage
         *
         * This actually deletes the object, and removes references
         * to it in various places.
         *
         * If the id doesn't exist, nothing will happen.
         *
         * \exsafe If an exception is thrown, the module instance is still
         *         removed from this database.
         *
         * \param [in] mb Pointer to the module to remove
         */
        void DeleteObject_(modulebase::ModuleBase * mb);


        /*! \brief Removes a created module object from storage
         *
         * This actually deletes the object, and removes references
         * to it in various places.
         *
         * If the id doesn't exist, nothing will happen.
         *
         * \exsafe If an exception is thrown, the module instance is still
         *         removed from this database.
         *
         * \param [in] id ID of the module to remove
         */
        void DeleteObject_(unsigned long id);

};



} // close namespace modulelocator
} // close namespace bpmodule


#endif
