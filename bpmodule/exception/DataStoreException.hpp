/*! \file
 *
 * \brief An exception for data storage (CalcData, etc)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_DATASTOREEXCEPTION_HPP_
#define _GUARD_DATASTOREEXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


/*! \brief An exception thrown when there is a problem with options
 *
 * This may be thrown on validation errors, etc.
 */
class DataStoreException : public GeneralException
{
    public:
        /*! \brief Constructor
         *
         * \param [in] whatstr Brief description of the error
         * \param [in] key  Key of the option
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        DataStoreException(std::string whatstr,
                        std::string key,
                        const Targs&... exinfo)

            : GeneralException(whatstr, "datakey", key, exinfo...)
        { }



        /*! \brief Constructor, using another exception as a base
         *
         * Can be used to append additional information
         *
         * \param [in] gex Exception to copy
         * \param [in] key Key causing the issue
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        DataStoreException(const GeneralException & gex, std::string key, const Targs&... exinfo)
            : GeneralException(gex, "datakey", key, exinfo...)
        { }


        DataStoreException()                                        = delete;
        DataStoreException(const DataStoreException & rhs)             = default;
        DataStoreException(DataStoreException && rhs)                  = default;
        DataStoreException & operator=(const DataStoreException & rhs) = default;
        DataStoreException & operator=(DataStoreException && rhs)      = default;
        virtual ~DataStoreException() = default;


        //! Return the key of the option with problems
        const char * Key(void) const noexcept     { return GeneralException::GetField("datakey");  }
};



}// close namespace exception
}// close namespace bpmodule



#endif
