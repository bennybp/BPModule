/*! \file
 *
 * \brief Storage of generic calculation data (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_CACHEDATA_HPP_
#define _GUARD_CACHEDATA_HPP_

#include <vector>

#include "bpmodule/datastore/PropertyMap.hpp"
#include "bpmodule/datastore/Hashable.hpp"

namespace bpmodule {
namespace datastore {


/*! \brief Class to hold arbitrary inputs and outputs
 */
class CacheData : public PropertyMap<std::string>
{
    public:
        typedef std::vector<const Hashable *> HashList;

        CacheData(void) = default;
        ~CacheData(void) = default;

        //! \copydoc PropertyMap::PropertyMap(const PropertyMap &)
        CacheData(const CacheData & rhs)             = default;

        //! \copydoc PropertyMap::PropertyMap(PropertyMap &&)
        CacheData(CacheData && rhs)                  = default;

        //! \copydoc PropertyMap::operator=(const PropertyMap &)
        CacheData & operator=(const CacheData & rhs) = default;

        //! \copydoc PropertyMap::operator=(PropertyMap &&)
        CacheData & operator=(CacheData && rhs)      = default;



        //! \copydoc bpmodule::datastore::PropertyMap::GetCopy
        template<typename T>
        T GetCopy(const std::string & key, const HashList & hashdata) const
        {
            return PropertyMap::GetCopy<T>(HashData_(key, hashdata));
        }


        //! \copydoc bpmodule::datastore::PropertyMap::GetRef
        template<typename T>
        const T & GetRef(const std::string & key, const HashList & hashdata) const
        {
            return PropertyMap::GetRef<T>(HashData_(key, hashdata));
        }


        //! \copydoc bpmodule::datastore::PropertyMap::Set
        template<typename T>
        void Set(const std::string & key, const T & value, const modulelocator::ModuleInfo & source,
                 const HashList & hashdata)
        {
            PropertyMap::Set(HashData_(key, hashdata), value, source);
        }


        //! \copydoc bpmodule::datastore::PropertyMap::Take
        template<typename T>
        void Take(const std::string & key, T && value, const modulelocator::ModuleInfo & source,
                  const HashList & hashdata)
        {
            PropertyMap::Take(HashData_(key, hashdata), std::move(value), source);
        }


        //! \copydoc bpmodule::datastore::PropertyMap::Erase
        size_t Erase(const std::string & key, const HashList & hashdata)
        {
            return PropertyMap::Erase(HashData_(key, hashdata));
        }


        //! \copydoc bpmodule::datastore::PropertyMap::SetRef(const PropertyMap &, const std::string &)
        void SetRef(const CacheData & other, const std::string & key, const HashList & hashdata)
        {
            PropertyMap::SetRef(other, HashData_(key, hashdata));
        }


        //! \copydoc bpmodule::datastore::PropertyMap::SetRef(const PropertyMap &, const std::string &, const std::string &)
        void SetRef(const CacheData & other, const std::string & key, const std::string & newkey,
                    const HashList & hashdata)
        {
            std::string srckey = HashData_(key, hashdata);
            std::string destkey = HashData_(newkey, hashdata);
            PropertyMap::SetRef(other, srckey, destkey);
        }


        ////////////////////////////////////
        // Python functions
        ////////////////////////////////////
        /*
        //! \copydoc bpmodule::datastore::PropertyMap::GetCopyPy
        boost::python::object GetCopyPy(const std::string & key) const
        {
            return PropertyMap::GetCopyPy(key);
        }

        //! \copydoc bpmodule::datastore::PropertyMap::SetPy
        void SetPy(const std::string & key, const boost::python::object & value, const modulelocator::ModuleInfo & source)
        {
            PropertyMap::SetPy(key, value, source);
        }
        */

    private:
        std::string HashData_(const std::string & key, const HashList & data) const
        {
            //! \todo wrap for exceptions
            std::string hash = key + "__";
            for(const auto & it : data)
                hash += it->Hash();
            return hash;
        }


};


} // close namespace datastore
} // close namespace bpmodule



#endif

