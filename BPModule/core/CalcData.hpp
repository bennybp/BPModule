#ifndef CALCDATA_HPP
#define CALCDATA_HPP

#include "BPModule/core/PropertyMap.hpp"

namespace bpmodule {


// Class holds inputs and outputs, allowing read-only access
// to stored data. ie, does not return non-const references
class CalcData : public PropertyMap
{
    public:
        CalcData(void) = default;
        ~CalcData(void) = default;

        CalcData(const CalcData & rhs) = default;
        CalcData & operator=(const CalcData & rhs) = default;
        CalcData(CalcData && rhs) = default;
        CalcData & operator=(CalcData && rhs) = default;


        // return a copy
        template<typename T>
        T GetCopy(const std::string & key) const
        {
            return PropertyMap::GetCopy<T>(key);
        }


        // return const ref
        template<typename T>
        const T & GetRef(const std::string & key) const
        {
            return PropertyMap::GetRef<T>(key);
        }

        // can add information
        template<typename T>
        void Set(const std::string & key, const T & value)
        {
            PropertyMap::Set(key, value);
        }

        template<typename T>
        void Take(const std::string & key, T && value)
        {
            PropertyMap::Take(key, std::move(value));
        }

        // and erase
        size_t Erase(const std::string & key)
        {
            return PropertyMap::Erase(key);
        }


        // add a reference to existing data
        void SetRef(const CalcData & ci, const std::string & key, const std::string & newkey)
        {
            PropertyMap::SetRef(ci, key, newkey);
        }

        void SetRef(const CalcData & ci, const std::string & key)
        {
            PropertyMap::SetRef(ci, key);
        }
};


} // close namespace bpmodule



#endif

