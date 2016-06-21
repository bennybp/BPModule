/*! \file
 *
 * \brief Storage and retrieval of command line arguments (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_UTIL__CMDLINE_HPP_
#define PULSAR_GUARD_UTIL__CMDLINE_HPP_

#include <vector>
#include <string>


namespace pulsar{
namespace util {

/*! Retrieves the number of arguments on the command line
 */
int * get_argc(void);


/*! Retrieves the command line
 */
char *** get_argv(void);

/*! Stores the command line (from python) for later use
 */
void set_cmdline(const std::vector<std::string> & argv);


/*! Frees memory associated with the stored command line
 */
void clear_cmdline(void);


} // close namespace util
} // close namespace pulsar


#endif
