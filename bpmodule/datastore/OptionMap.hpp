/*! \file
 *
 * \brief Storage of options for a module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_OPTIONMAP_HPP_
#define _GUARD_OPTIONMAP_HPP_

#include "bpmodule/datastore/PropertyMap.hpp"


namespace bpmodule {
namespace datastore {


/*! \brief A class for holding options to a module
 *
 * This is constructed (probably from python) and then read-only. 
 * All the Get, etc, members of PropertyMap are protected, and
 * are exposed selectively
 */
class OptionMap : public PropertyMap
{
    public:
        OptionMap(void) = default;
        ~OptionMap(void) = default;

        //! \copydoc PropertyMap::PropertyMap(const PropertyMap &)
        OptionMap(const OptionMap & rhs)             = default;

        //! \copydoc PropertyMap::PropertyMap(PropertyMap &&)
        OptionMap(OptionMap && rhs)                  = default;

        //! \copydoc PropertyMap::operator=(const PropertyMap &)
        OptionMap & operator=(const OptionMap & rhs) = default;

        //! \copydoc PropertyMap::operator=(PropertyMap &&)
        OptionMap & operator=(OptionMap && rhs)      = default;


        /*! \brief Construct options from a python object
         *
         * \copydetails PropertyMap::PropertyMap(const boost::python::list &)
         */ 
        OptionMap(const boost::python::list & olist) : PropertyMap(olist) {  }


        //! \copydoc PropertyMap::GetCopy 
        template<typename T>
        T Get(const std::string & key) const
        {
            return PropertyMap::GetCopy<T>(key);
        }
};


} // close namespace datastore
} // close namespace bpmodule



#endif

