/*! \file
 *
 * \brief OutputStream class (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_OUTPUT__OUTPUTTYPE_HPP_
#define PULSAR_GUARD_OUTPUT__OUTPUTTYPE_HPP_

namespace pulsar{

/*! The type of information being output
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

} // close namespace pulsar


#endif
