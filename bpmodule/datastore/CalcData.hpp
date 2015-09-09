/*! \file
 *
 * \brief Storage of generic calculation data (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_CALCDATA_HPP_
#define _GUARD_CALCDATA_HPP_

#include "bpmodule/datastore/PropertyMap.hpp"

namespace bpmodule {
namespace datastore {


/*! \brief Class to hold arbitrary inputs and outputs
 *
 *  A CalcData object can hold arbitrary data types and is
 *  used primarily to pass information to and from modules.
 *  Access to the underlying data is NOT allowed, ensuring well-defined
 *  behavior when two modules are using the same underlying data.
 *
 *  This implements a somewhat copy-on-write scheme, where replacing
 *  the data will cause the data to be replaced only in a particular
 *  object, while other objects will still retain the original data.
 *  This is done through the use of std::shared_ptr.
 *
 *  See \ref developer_calcdata_sec "CalcData development notes"
 */
class CalcData : public PropertyMap
{
    public:
        CalcData(void) = default;
        ~CalcData(void) = default;

        //! \copydoc PropertyMap::PropertyMap(const PropertyMap &)
        CalcData(const CalcData & rhs)             = default;

        //! \copydoc PropertyMap::PropertyMap(PropertyMap &&)
        CalcData(CalcData && rhs)                  = default;

        //! \copydoc PropertyMap::operator=(const PropertyMap &)
        CalcData & operator=(const CalcData & rhs) = default;

        //! \copydoc PropertyMap::operator=(PropertyMap &&)
        CalcData & operator=(CalcData && rhs)      = default;



        //! \copydoc bpmodule::datastore::PropertyMap::GetCopy
        template<typename T>
        T GetCopy(const std::string & key) const
        {
            return PropertyMap::GetCopy<T>(key);
        }


        //! \copydoc bpmodule::datastore::PropertyMap::GetRef
        template<typename T>
        const T & GetRef(const std::string & key) const
        {
            return PropertyMap::GetRef<T>(key);
        }


        //! \copydoc bpmodule::datastore::PropertyMap::Set
        template<typename T>
        void Set(const std::string & key, const T & value)
        {
            PropertyMap::Set(key, value);
        }


        //! \copydoc bpmodule::datastore::PropertyMap::Take
        template<typename T>
        void Take(const std::string & key, T && value)
        {
            PropertyMap::Take(key, std::move(value));
        }


        //! \copydoc bpmodule::datastore::PropertyMap::Erase
        size_t Erase(const std::string & key)
        {
            return PropertyMap::Erase(key);
        }


        //! \copydoc bpmodule::datastore::PropertyMap::SetRef(const PropertyMap &, const std::string &)
        void SetRef(const CalcData &other, const std::string & key)
        {
            PropertyMap::SetRef(other, key);
        }


        //! \copydoc bpmodule::datastore::PropertyMap::SetRef(const PropertyMap &, const std::string &, const std::string &)
        void SetRef(const CalcData & other, const std::string & key, const std::string & newkey)
        {
            PropertyMap::SetRef(other, key, newkey);
        }

};


} // close namespace datastore
} // close namespace bpmodule



#endif

