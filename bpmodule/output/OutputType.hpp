/*! \file
 *
 * \brief OutputStream class (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_OUTPUT__OUTPUTTYPE_HPP_
#define BPMODULE_GUARD_OUTPUT__OUTPUTTYPE_HPP_

namespace bpmodule {
namespace output {


/*! The type of information being output
 *
 * \warning Internal use only
 */
enum class OutputType
{
    Output,  //!< General output
    Success, //!< Something has succeeded
    Changed, //!< Something has been changed (ie from defaults)
    Warning, //!< Something is kinda wrong
    Error,   //!< Something is very wrong
    Debug    //!< For developers
};


} // close namespace output
} // close namespace bpmodule


#endif
