/*! \file
 *
 * \brief Storage and retrieval of command line arguments (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_CMDLINE_HPP_
#define _GUARD_CMDLINE_HPP_

#include <vector>
#include <string>

#include "bpmodule/python_helper/Pybind11.hpp"

namespace bpmodule {
namespace util {

/*! Retrieves the number of arguments on the command line
 */
int * GetArgc(void);


/*! Retrieves the command line
 */
char *** GetArgv(void);

/*! Stores the command line (from python) for later use
 */
void SetCmdline(pybind11::object argv);


/*! Frees memory associated with the stored command line
 */
void ClearCmdline(void);


} // close namespace util
} // close namespace bpmodule


#endif
