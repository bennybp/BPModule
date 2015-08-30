#ifndef OPTIONMAP_HPP
#define OPTIONMAP_HPP

#include "bpmodule/datastore/PropertyMap.hpp"


namespace bpmodule {
namespace datastore {


// This is constructed (probably from python) and then read-only 
// All the Get, etc, members of PropertyMap are protected, and
// are exposed selectively
// Has, GetType, etc, are public
class OptionMap : public PropertyMap
{
    public:
        OptionMap(void) = default;
        ~OptionMap(void) = default;

        // Options can be copied
        OptionMap(const OptionMap & rhs) = default; // just needs base class copy constructor
        OptionMap & operator=(const OptionMap & rhs) = default;

        OptionMap(OptionMap && rhs) = default;
        OptionMap & operator=(OptionMap && rhs) = default;

        // Construct from python
        OptionMap(const boost::python::list & olist) : PropertyMap(olist) {  }

        // return a copy
        template<typename T>
        T Get(const std::string & key) const
        {
            return PropertyMap::GetCopy<T>(key);
        }
};


} // close namespace datastore
} // close namespace bpmodule



#endif

