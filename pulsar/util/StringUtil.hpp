/*! \file
 *
 * \brief Various string helper functions (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_UTIL__STRINGUTIL_HPP_
#define PULSAR_GUARD_UTIL__STRINGUTIL_HPP_

#include <string>


namespace pulsar{
namespace util {

/*! \brief Joins each element of a container of strings
 *
 * \tparam A container type (vector, set) of strings
 * \param [in] container A container of strings to join
 * \param [in] j         What to put in between each string
 * \return A string with each element of \p container joined with \p j in between
 */
template<typename T>
std::string join(const T & container, const std::string & j)
{
    if(container.size() == 0)
        return std::string();


    auto it = container.begin();
    std::string str = *it;
    std::advance(it, 1);

    for(; it != container.end(); ++it)
    {
        str.append(j);
        str.append(*it);
    }

    return str;
}


/*! \brief Transform a string to lower case
 *
 * \param [inout] str String to convert to lowercase
 */
void to_lower(std::string & str);


/*! \brief Transform a string to lower case, copying to a new string
 *
 * \param [in] str String to convert to lowercase
 * \return The string converted to lowercase
 */
std::string to_lower_copy(std::string str);


/*! \brief trim a string (beginning)
 * 
 * \param [in] s String to trim
 */
void left_trim(std::string & s);


/*! \brief trim a string (ending)
 * 
 * \param [in] s String to trim
 */
void right_trim(std::string & s);


/*! \brief trim a string (beginning and ending)
 * 
 * \param [in] s String to trim
 */
void trim(std::string & s);


/*! \brief trim a string (beginning)
 * 
 * \param [in] s String to trim
 * \return Copy of s with leading whitespace removed
 */
std::string left_trim_copy(std::string s);


/*! \brief trim a string _copy(ending)
 * 
 * \param [in] s String to trim
 * \return Copy of s with trailing whitespace removed
 */
std::string right_trim_copy(std::string s);


/*! \brief trim a string _copy(beginning and ending)
 * 
 * \param [in] s String to trim
 * \return Copy of s with leading and trailing whitespace removed
 */
std::string trim_copy(std::string s);



/*! \brief Less-than comparison of a case-insensitive string
 *
 * Useful for containers (maps) where the key is
 * case insensitive
 */
struct CaseInsensitiveLess
{
    bool operator()(std::string lhs, std::string rhs) const;
}; 


/*! \brief Less-than comparison of a case-insensitive string, trimming leading and trailing whitespace
 *
 * Useful for containers (maps) where the key is
 * case insensitive
 */
struct CaseInsensitivetrimLess
{
    bool operator()(std::string lhs, std::string rhs) const;
}; 


/*! \brief Equality comparison of a case-insensitive string
 *
 * Useful for containers (maps) where the key is
 * case insensitive
 */
struct CaseInsensitiveEquality
{
    bool operator()(std::string lhs, std::string rhs) const;
}; 


/*! \brief Equality comparison of a case-insensitive string, trimming leading and trailing whitespace
 */
struct CaseInsensitivetrimEquality
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
std::string line(char c, int n = 80);


} // close namespace util
} // close namespace pulsar

#endif

