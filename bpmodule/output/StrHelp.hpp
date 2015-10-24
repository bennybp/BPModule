/*! \file
 *
 * \brief Various string helper functions (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_STRHELP_HPP_
#define _GUARD_STRHELP_HPP_

#include <vector>



/*! \brief Joins each element of a vector of strings
 *
 * \param [in] vec A vector of strings to join
 * \param [in] j   What to put in between each string
 * \return A string with each element of \p vec joined with \p j in between
 */
std::string Join(const std::vector<std::string> & vec, const std::string & j);



#endif
