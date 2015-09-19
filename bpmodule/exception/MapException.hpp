/*! \file
 *
 * \brief An exception for map/dictionary errors
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_MAPEXCEPTION_HPP_
#define _GUARD_MAPEXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


/*! \brief An exception thrown when a map has a problem 
 *
 * Missing key, etc
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
        
        
        /*! \brief Constructor for bad casts
         *
         * \param [in] what Brief description of the error
         * \param [in] map  What map had a problem
         * \param [in] key  Key that caused the problem
         * \param [in] from Source data type
         * \param [in] to   Destination data type
         * \param [in] desc Additional information
         */
        MapException(std::string what,
                     std::string map,
                     std::string key,
                     std::string from,
                     std::string to,
                     std::string desc = "(no details)")
            : GeneralException(what,
                               {
                                  {"map", map},
                                  {"key", key},
                                  {"from", from},
                                  {"to", to},
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
        const char * Map(void) const noexcept      { return GeneralException::GetField("map"); }

        //! Return the key that caused the probelm
        const char * Key(void) const noexcept      { return GeneralException::GetField("key");  }

        //! Return the source data type
        const char * From(void) const noexcept     { return GeneralException::GetField("from");  }

        //! Return the destination data type
        const char * To(void) const noexcept       { return GeneralException::GetField("to");  }

        //! Return any additional details
        const char * Desc(void) const noexcept     { return GeneralException::GetField("desc"); }
};



}// close namespace bpmodule
}// close namespace exception



#endif
