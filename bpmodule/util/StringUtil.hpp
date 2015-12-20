/*! \file
 *
 * \brief Various string helper functions (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_UTIL__STRINGUTIL_HPP_
#define BPMODULE_GUARD_UTIL__STRINGUTIL_HPP_

#include <vector>
#include <string>

namespace bpmodule {
namespace util {

/*! \brief Joins each element of a vector of strings
 *
 * \param [in] vec A vector of strings to join
 * \param [in] j   What to put in between each string
 * \return A string with each element of \p vec joined with \p j in between
 */
std::string Join(const std::vector<std::string> & vec, const std::string & j);


/*! \brief Transform a string to lower case
 *
 * \param [inout] str String to convert to lowercase
 */
void ToLower(std::string & str);


/*! \brief Transform a string to lower case, copying to a new string
 *
 * \param [in] str String to convert to lowercase
 * \return The string converted to lowercase
 */
std::string ToLowerCopy(std::string str);



/*! \brief Comparison of a case-insensitive string
 *
 * Useful for containers (maps) where the key is
 * case insensitive
 */
struct CaseInsensitiveCompare
{
    bool operator()(std::string lhs, std::string rhs) const;
}; 



/*! \brief Create a line of characters
 *
 * Repeats a character a number of times. A newline is included.
 *
 * \param [in] c The character to use
 * \param [in] n The number of times to repeat the character
 */
std::string Line(char c, int n = 80);




} // close namespace util
} // close namespace bpmodule

#endif

