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

        // CalcData cannot be copied
        CalcData(const CalcData & rhs) = delete;
        CalcData & operator=(const CalcData & rhs) = delete;
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
        void Add(const std::string & key, const T & value)
        {
            PropertyMap::Add(key, value);
        }

        template<typename T>
        void Add(const std::string & key, T && value)
        {
            PropertyMap::Add(key, std::move(value));
        }

        // can replace information
        template<typename T>
        void Replace(const std::string & key, const T & value)
        {
            PropertyMap::Replace(key, value);
        }

        template<typename T>
        void Replace(const std::string & key, T && value)
        {
            PropertyMap::Replace(key, std::move(value));
        }

        // and erase
        size_t Erase(const std::string & key)
        {
            return PropertyMap::Erase(key);
        }


        // add a reference to existing data
        void AddRef(const CalcData & ci, const std::string & key)
        {
            PropertyMap::AddRef(ci, key);
        }
};


// some typedefs
typedef CalcData CalcInputs;
typedef CalcData CalcResults;


} // close namespace bpmodule



#endif

