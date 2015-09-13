#ifndef _GUARD_MAPEXCEPTION_HPP_
#define _GUARD_MAPEXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


/*! \brief An exception thrown when ModuleStore has a problem 
 *
 * Particularly when creating modules or looking up information keys
 */
class MapException : public GeneralException
{
    public:
        /*! \brief Constructor
         *
         * \param [in] what Brief description of the error
         * \param [in] map  What map had a problem
         * \param [in] key  Key that caused the problem
         * \param [in] desc Additional information
         */
        MapException(std::string what,
                     std::string map,
                     std::string key,
                     std::string desc = "(no details)")
            : GeneralException(what,
                               {
                                  {"map", map},
                                  {"key", key},
                                  {"desc", desc}
                               })
                { }
                
        
        MapException()                                     = delete;     
        MapException(const MapException & rhs)             = default;     
        MapException(MapException && rhs)                  = default;     
        MapException & operator=(const MapException & rhs) = default;     
        MapException & operator=(MapException && rhs)      = default;     
        virtual ~MapException() = default;


        //! Return what map had the problem
        const char * Map(void) const noexcept     { return GeneralException::GetField("map"); }

        //! Return the key that caused the probelm
        const char * Key(void) const noexcept     { return GeneralException::GetField("key");  }

        //! Return any additional details
        const char * Desc(void) const noexcept    { return GeneralException::GetField("desc"); }
};



}// close namespace bpmodule
}// close namespace exception



#endif
